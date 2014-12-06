// ==========================================================================
// Dedmonwakeen's Raid DPS/TPS Simulator.
// Send questions to natehieter@gmail.com
// ==========================================================================

#include "simulationcraft.hpp"

#include "util/rapidjson/document.h"
#include "util/rapidjson/stringbuffer.h"
#include "util/rapidjson/prettywriter.h"

// ==========================================================================
// Blizzard Community Platform API
// ==========================================================================

namespace { // UNNAMED NAMESPACE

player_e parse_armory_class( const std::string& class_str )
{
  player_e pt = util::translate_class_str( class_str );
  if ( pt == PLAYER_NONE )
  {
    if ( util::str_compare_ci( class_str, "death-knight" ) )
      pt = DEATH_KNIGHT;
  }

  return pt;
}

race_e parse_armory_race( const std::string& race_str )
{
  race_e rt = util::parse_race_type( race_str );
  if ( rt == RACE_NONE )
  {
    if ( util::str_compare_ci( race_str, "blood-elf" ) )
      rt = RACE_BLOOD_ELF;
    else if ( util::str_compare_ci( race_str, "night-elf" ) )
      rt = RACE_NIGHT_ELF;
    // Armory HTML does not give an url to the racial section of Pandarens ..
    // instead it contains an empty javascript entry. The rest of the races
    // contain proper urls.
    else if ( util::str_compare_ci( race_str, "javascript:;" ) )
      rt = RACE_PANDAREN;
  }

  return rt;
}

struct player_spec_t
{
  std::string region, server, name, url, local_json, origin, talent_spec;
};

// download_id ==============================================================

bool download_id( rapidjson::Document& d, 
                  const std::string& region,
                  unsigned item_id,
                  cache::behavior_e caching )
{
  if ( item_id == 0 )
    return false;

  std::string url = "http://" + region + ".battle.net/api/wow/item/" + util::to_string( item_id ) + "?locale=en_US";

  std::string result;
  if ( ! http::get( result, url, caching ) )
    return false;

  d.Parse< 0 >( result.c_str() );
  return true;
}

// parse_profession =========================================================

void parse_profession( std::string&               professions_str,
                       const rapidjson::Value&    profile,
                       int                        index )
{
  if ( ! profile.HasMember( "primary" ) )
    return;

  const rapidjson::Value& professions = profile[ "primary" ];

  if ( professions.Size() >= rapidjson::SizeType( index + 1 ) )
  {
    const rapidjson::Value& profession = professions[ index ];
    if ( ! profession.HasMember( "id" ) || ! profession.HasMember( "rank" ) )
      return;

    if ( professions_str.length() > 0 )
      professions_str += '/';

    professions_str += util::profession_type_string( util::translate_profession_id( profession[ "id" ].GetUint() ) );
    professions_str += "=";
    professions_str += util::to_string( profession[ "rank" ].GetUint() );
  }
}

// parse_talents ============================================================

const rapidjson::Value* choose_talent_spec( const rapidjson::Value& talents,
                                                   const std::string& specifier )
{
  const rapidjson::Value& spec_1 = talents[ 0u ];
  const rapidjson::Value& spec_2 = talents[ 1u ];
  bool spec1_is_active = false;
  if ( spec_1.HasMember( "selected" ) )
    spec1_is_active = spec_1[ "selected" ].GetBool();

  const rapidjson::Value* spec = 0;

  if ( util::str_compare_ci( specifier, "active" ) )
    spec = spec1_is_active ? &spec_1 : &spec_2;
  else if ( util::str_compare_ci( specifier, "inactive" ) )
    spec = spec1_is_active ? &spec_2 : &spec_1;
  else if ( util::str_compare_ci( specifier, "primary" ) )
    spec = &spec_1;
  else if ( util::str_compare_ci( specifier, "secondary" ) )
    spec = &spec_2;

  return spec;
}

bool parse_talents( player_t*  p,
                    const rapidjson::Value& talents,
                    const std::string& specifier )
{
  if ( talents.Size() != 2 )
    return false;

  const rapidjson::Value* spec = choose_talent_spec( talents, specifier );
  if ( ! spec )
    return false;

  if ( ! spec -> HasMember( "calcSpec" ) || ! spec -> HasMember( "calcTalent" ) )
    return false;

  const char* buffer = (*spec)[ "calcSpec" ].GetString();
  unsigned sid;
  switch ( buffer[ 0 ] )
  {
    case 'a': sid = 0; break;
    case 'Z': sid = 1; break;
    case 'b': sid = 2; break;
    case 'Y': sid = 3; break;
    default:  sid = 99; break;
  }
  p -> _spec = p -> dbc.spec_by_idx( p -> type, sid );

  std::string talent_encoding = (*spec)[ "calcTalent" ].GetString();

  for ( size_t i = 0; i < talent_encoding.size(); ++i )
  {
    switch ( talent_encoding[ i ] )
    {
      case '.':
        talent_encoding[ i ] = '0';
        break;
      case '0':
      case '1':
      case '2':
        talent_encoding[ i ] += 1;
        break;
      default:
        p -> sim -> errorf( "BCP API: Invalid character '%c' in talent encoding for player %s.\n", talent_encoding[ i ], p -> name() );
        return false;
    }
  }

  if ( ! p -> parse_talents_numbers( talent_encoding ) )
  {
    p -> sim -> errorf( "BCP API: Can't parse talent encoding '%s' for player %s.\n", talent_encoding.c_str(), p -> name() );
    return false;
  }

  p -> create_talents_armory();

  return true;
}

// parse_glyphs =============================================================

bool parse_glyphs( player_t* p,
                   const rapidjson::Value& talents,
                   const std::string& specifier )
{
  static const char* const glyph_e_names[] =
  {
    "major", "minor"
  };

  const rapidjson::Value* spec = choose_talent_spec( talents, specifier );
  if ( ! spec || ! spec -> HasMember( "glyphs" ) )
    return false;

  const rapidjson::Value& glyphs = (*spec)[ "glyphs" ];

  for ( std::size_t i = 0; i < sizeof_array( glyph_e_names ); ++i )
  {
    if ( ! glyphs.HasMember( glyph_e_names[ i ] ) )
      continue;

    const rapidjson::Value& glyphs_type = glyphs[ glyph_e_names[ i ] ];
    for ( rapidjson::SizeType j = 0; j < glyphs_type.Size(); j++ )
    {
      std::string glyph_name;
      if ( glyphs_type[ j ].HasMember( "name" ) )
          glyph_name = glyphs_type[ j ][ "name" ].GetString();
      else if ( glyphs_type[ j ].HasMember( "item" ) )
        item_t::download_glyph( p, glyph_name,
                                util::to_string( glyphs_type[ j ][ "item" ].GetString() ) );

      util::glyph_name( glyph_name );
      if ( ! glyph_name.empty() )
      {
        if ( ! p -> glyphs_str.empty() )
          p -> glyphs_str += '/';
        p -> glyphs_str += glyph_name;
      }
    }
  }

  return true;
}

// parse_items ==============================================================

bool parse_items( player_t*  p,
                  const rapidjson::Value& items )
{
  static const char* const slot_map[] =
  {
    "head",
    "neck",
    "shoulder",
    "shirt",
    "chest",
    "waist",
    "legs",
    "feet",
    "wrist",
    "hands",
    "finger1",
    "finger2",
    "trinket1",
    "trinket2",
    "back",
    "mainHand",
    "offHand",
    "ranged",
    "tabard"
  };

  assert( sizeof_array( slot_map ) == SLOT_MAX );

  for ( unsigned i = 0; i < SLOT_MAX; ++i )
  {
    item_t& item = p -> items[ i ];

    if ( ! items.HasMember( slot_map[ i ] ) )
      continue;

    const rapidjson::Value& data = items[ slot_map[ i ] ];
    if ( ! data.HasMember( "id" ) )
      continue;
    else
      item.parsed.data.id = data[ "id" ].GetUint();

    if ( data.HasMember( "bonusLists" ) )
    {
      for ( rapidjson::SizeType k = 0, n = data[ "bonusLists" ].Size(); k < n; ++k )
      {
        item.parsed.bonus_id.push_back( data[ "bonusLists" ][ k ].GetInt() );
      }
    }

    if ( ! data.HasMember( "tooltipParams" ) )
      continue;

    const rapidjson::Value& params = data[ "tooltipParams" ];

    if ( params.HasMember( "gem0" ) ) item.parsed.gem_id[ 0 ] = params[ "gem0" ].GetUint();
    if ( params.HasMember( "gem1" ) ) item.parsed.gem_id[ 1 ] = params[ "gem1" ].GetUint();
    if ( params.HasMember( "gem2" ) ) item.parsed.gem_id[ 2 ] = params[ "gem2" ].GetUint();
    if ( params.HasMember( "enchant" ) ) item.parsed.enchant_id = params[ "enchant" ].GetUint();
    if ( params.HasMember( "tinker" ) ) item.parsed.addon_id = params[ "tinker" ].GetUint();
    if ( params.HasMember( "suffix" ) ) item.parsed.suffix_id = params[ "suffix" ].GetInt();

    if ( params.HasMember( "upgrade" ) )
    {
      const rapidjson::Value& upgrade = params[ "upgrade" ];
      if ( upgrade.HasMember( "current" ) ) item.parsed.upgrade_level = upgrade[ "current" ].GetUint();
    }
  }

  return true;
}

// parse_player =============================================================

bool parse_player_html_talent_glyph( sim_t*,
                                     const std::string& specifier,
                                     player_t*          player,
                                     const sc_xml_t&    data )
{

  sc_xml_t specs_obj = data.get_node( "div", "class", "talent-specs" );
  if ( ! specs_obj.valid() )
  {
    return false;
  }

  sc_xml_t spec_0_obj = specs_obj.get_node( "a", "data-spec-id", "0" );
  sc_xml_t spec_1_obj = specs_obj.get_node( "a", "data-spec-id", "1" );
  std::string spec_0_str, spec_1_str;
  if ( ! spec_0_obj.valid() || ! spec_0_obj.get_value( spec_0_str, "class" ) )
  {
    return false;
  }

  if ( ! spec_1_obj.valid() || ! spec_1_obj.get_value( spec_1_str, "class" ) )
  {
    return false;
  }

  bool spec0_active = util::str_in_str_ci( spec_0_str, "active" );
  std::string spec_id;

  if ( util::str_compare_ci( specifier, "active" ) )
  {
    spec_id = spec0_active ? "0" : "1";
  }
  else if ( util::str_compare_ci( specifier, "inactive" ) )
  {
    spec_id = spec0_active ? "1" : "0";
  }
  else if ( util::str_compare_ci( specifier, "primary" ) )
  {
    spec_id = "0";
  }
  else if ( util::str_compare_ci( specifier, "secondary" ) )
  {
    spec_id = "1";
  }
  else
    return false;

  // Figure out the canonical specialization name from the active talent spec
  std::string spec_name;
  sc_xml_t spec_obj = specs_obj.get_node( "a", "data-spec-id", spec_id );
  if ( ! spec_obj.valid() || ! spec_obj.get_value( spec_name, "data-spec-name" ) )
  {
    return false;
  }

  // ..aand construct a hacky string that we can match to a specialization_e
  // enum ...
  std::string profile_spec = spec_name + " ";
  profile_spec += util::player_type_string( player -> type );
  player -> _spec = util::parse_specialization_type( profile_spec );
  if ( player -> _spec == SPEC_NONE )
  {
    player -> sim -> errorf( "BCP API: Can't parse specialization '%s' for player %s.\n",
        profile_spec.c_str(),
        player -> name() );
  }

  // Parse through talent tiers
  sc_xml_t talents_obj = data.get_node( "div", "id", "talent-build-" + spec_id );
  if ( ! talents_obj.valid() )
  {
    return false;
  }

  std::vector<sc_xml_t> talent_tiers_obj = talents_obj.get_nodes( "li", "class", "talent" );
  std::vector<std::string> talent_arr;
  std::string talents_str;
  for ( size_t i = 0; i < talent_tiers_obj.size(); i++ )
  {
    int tier, column;
    if ( ! talent_tiers_obj[ i ].get_value( tier, "data-tier" ) ||
         ! talent_tiers_obj[ i ].get_value( column, "data-column" ) )
    {
      return false;
    }

    if ( talent_arr.size() < static_cast<size_t>( tier + 1 ) )
      talent_arr.resize( tier + 1, "0" );
    talent_arr[ tier ] = util::to_string( column + 1 );
  }

  // Aand construct a talent string, that will be parsed by magic.
  for ( size_t i = 0; i < talent_arr.size(); i++ )
  {
    talents_str += talent_arr[ i ];
  }

  if ( ! player -> parse_talents_numbers( talents_str ) )
  {
    player -> sim -> errorf( "BCP API: Can't parse talent encoding '%s' for player %s.\n",
        talents_str.c_str(),
        player -> name() );
    return false;
  }

  // And re-format talents into an armory-based talent url, since this is
  // armory import
  player -> create_talents_armory();

  // And then parse glyphs
  sc_xml_t glyphs_obj = talents_obj.get_node( "div", "class", "glyphs" );
  if ( ! glyphs_obj.valid() )
  {
    return false;
  }

  // Get all "<div id="tooltip-X">" elements ...
  std::vector<sc_xml_t> glyph_objs = glyphs_obj.get_nodes( "ul/li/div" );
  std::string seek_str = "tooltip-";
  for ( size_t i = 0; i < glyph_objs.size(); i++ )
  {
    std::string glyph_data_str;
    if ( ! glyph_objs[ i ].get_value( glyph_data_str, "id" ) )
    {
      continue;
    }

    // Yank the glyph spell id from the data, data is in format "tooltip-<id>"
    std::string::size_type pos = glyph_data_str.find( seek_str );
    if ( pos == std::string::npos )
    {
      continue;
    }

    if ( ! player -> glyphs_str.empty() )
    {
      player -> glyphs_str += "/";
    }

    unsigned glyph_id = util::to_unsigned( glyph_data_str.substr( seek_str.size() ) );
    std::string glyph_name = player -> find_spell( glyph_id ) -> name_cstr();
    if ( ! glyph_name.empty() )
    {
      util::glyph_name( glyph_name );
    }

    player -> glyphs_str += glyph_name;
  }

  return true;
}

bool parse_player_html_profession( sim_t*,
                                   player_t*       player,
                                   const sc_xml_t& data )
{

  sc_xml_t prof_obj = data.get_node( "div", "class", "summary-professions" );
  if ( ! prof_obj.valid() )
  {
    return false;
  }

  std::vector<sc_xml_t> profession_nodes = prof_obj.get_nodes( "a", "class", "profession-details" );
  for ( size_t i = 0; i < profession_nodes.size(); i++ )
  {
    std::string profession_href, profession_value;

    if ( ! profession_nodes[ i ].get_value( profession_href, "href" ) )
    {
      continue;
    }

    sc_xml_t value_obj = profession_nodes[ i ].get_node( "span", "class", "value" );
    if ( ! value_obj.valid() || ! value_obj.get_value( profession_value, "." ) )
    {
      continue;
    }

    if ( ! player -> professions_str.empty() )
    {
      player -> professions_str += "/";
    }

    std::vector<std::string> profession_href_split = util::string_split( profession_href, "/" );
    player -> professions_str += profession_href_split.back();
    player -> professions_str += "=";
    player -> professions_str += profession_value;
  }

  return true;
}

player_t* parse_player_html( sim_t*             sim,
                             player_spec_t&     player,
                             cache::behavior_e  caching )
{
  sim -> current_slot = 0;

  sc_xml_t profile = sc_xml_t::get( sim, player.url, caching );
  if ( ! profile.valid() )
  {
    sim -> errorf( "BCP API: Unable to download player from '%s'\n", player.url.c_str() );
    return 0;
  }

  if ( sim -> debug && profile.valid() )
  {
    profile.print();
  }

  sc_xml_t name_obj = profile.get_node( "div", "class", "name" );
  if ( ! name_obj.valid() || ! name_obj.get_value( player.name, "a/." ) )
  {
    sim -> errorf( "BCP API: Unable to extract player name from '%s'.\n", player.url.c_str() );
    return 0;
  }

  sc_xml_t class_obj = profile.get_node( "a", "class", "class" );
  std::string class_name_data, class_name;
  player_e class_type = PLAYER_NONE;
  if ( ! class_obj.valid() || ! class_obj.get_value( class_name_data, "href" ) )
  {
    sim -> errorf( "BCP API: Unable to extract player class from '%s'.\n", player.url.c_str() );
    return 0;
  }
  else
  {
    std::vector<std::string> class_split = util::string_split( class_name_data, "/" );
    class_name = class_split[ class_split.size() - 1 ];
    class_type = parse_armory_class( class_name );
  }

  const module_t* module = module_t::get( class_type );
  if ( ! module || ! module -> valid() )
  {
    sim -> errorf( "\nModule for class %s is currently not available.\n", class_name.c_str() );
    return 0;
  }

  sc_xml_t level_obj = profile.get_node( "span", "class", "level" );
  int level = 0;
  if ( ! level_obj.valid() || ! level_obj.get_value( level, "strong/." ) )
  {
    sim -> errorf( "BCP API: Unable to extract player level from '%s'.\n", player.url.c_str() );
    return 0;
  }

  sc_xml_t race_obj = profile.get_node( "a", "class", "race" );
  std::string race_name_data, race_name;
  race_e race_type = RACE_NONE;
  if ( ! race_obj.valid() || ! race_obj.get_value( race_name_data, "href" ) )
  {
    sim -> errorf( "BCP API: Unable to extract player race from '%s'.\n", player.url.c_str() );
    return 0;
  }
  else
  {
    std::vector<std::string> race_split = util::string_split( race_name_data, "/" );
    race_name = race_split[ race_split.size() - 1 ];
    race_type = parse_armory_race( race_name );
  }

  if ( race_type == RACE_NONE )
  {
    sim -> errorf( "BCP API: Unable to extract player race from '%s'.\n", player.url.c_str() );
    return 0;
  }

  std::string name = player.name;

  if ( player.talent_spec != "active" && ! player.talent_spec.empty() )
  {
    name += '_';
    name += player.talent_spec;
  }

  if ( ! name.empty() )
    sim -> current_name = name;

  player_t* p = sim -> active_player = module -> create_player( sim, name, race_type );

  if ( ! p )
  {
    sim -> errorf( "BCP API: Unable to build player with class '%s' and name '%s' from '%s'.\n",
                   class_name.c_str(), name.c_str(), player.url.c_str() );
    return 0;
  }

  p -> level = level;
  p -> region_str = player.region.empty() ? sim -> default_region_str : player.region;
  sc_xml_t realm_obj = profile.get_node( "span", "id", "profile-info-realm" );
  if ( ! realm_obj.valid() )
  {
    if ( ! player.server.empty() )
      p -> server_str = player.server;
  }
  else
  {
    realm_obj.get_value( p -> server_str, "." );
  }

  if ( ! player.origin.empty() )
    p -> origin_str = player.origin;

  // TODO: Do we need to error check this nowadays?
  parse_player_html_profession( sim, p, profile );
  if ( ! parse_player_html_talent_glyph( sim, player.talent_spec, p, profile ) )
  {
    sim -> errorf( "BCP API: Unable to extract player talent specialization from '%s'.\n",
        player.url.c_str() );
    return 0;
  }

  return p;
}

// parse_player =============================================================

player_t* parse_player( sim_t*             sim,
                        player_spec_t&     player,
                        cache::behavior_e  caching )
{
  sim -> current_slot = 0;

  std::string result;

  if ( player.local_json.empty() )
  {
    if ( ! http::get( result, player.url, caching ) )
      return 0;
  }
  else
  {
    io::ifstream ifs;
    ifs.open( player.local_json );
    result.assign( ( std::istreambuf_iterator<char>( ifs ) ),
                   ( std::istreambuf_iterator<char>()    ) );
  }

  rapidjson::Document profile;
  profile.Parse< 0 >( result.c_str() );

  if ( profile.HasParseError() )
  {
    sim -> errorf( "BCP API: Unable to download player from '%s'\n", player.url.c_str() );
    return 0;
  }

  if ( sim -> debug )
  {
    rapidjson::StringBuffer b;
    rapidjson::PrettyWriter< rapidjson::StringBuffer > writer( b );

    profile.Accept( writer );
    sim -> out_debug.raw() << b.GetString();
  }

  if ( profile.HasMember( "status" ) && util::str_compare_ci( profile[ "status" ].GetString(), "nok" ) )
  {
    sim -> errorf( "BCP API: Unable to download player from '%s', reason: %s\n",
                   player.url.c_str(),
                   profile[ "reason" ].GetString() );
    return 0;
  }

  if ( profile.HasMember( "name" ) )
    player.name = profile[ "name" ].GetString();

  if ( ! profile.HasMember( "level" ) )
  {
    sim -> errorf( "BCP API: Unable to extract player level from '%s'.\n", player.url.c_str() );
    return 0;
  }

  if ( ! profile.HasMember( "class" ) )
  {
    sim -> errorf( "BCP API: Unable to extract player class from '%s'.\n", player.url.c_str() );
    return 0;
  }

  if ( ! profile.HasMember( "race" ) )
  {
    sim -> errorf( "BCP API: Unable to extract player race from '%s'.\n", player.url.c_str() );
    return 0;
  }

  if ( ! profile.HasMember( "talents" ) )
  {
    sim -> errorf( "BCP API: Unable to extract player talents from '%s'.\n", player.url.c_str() );
    return 0;
  }

  std::string class_name = util::player_type_string( util::translate_class_id( profile[ "class" ].GetUint() ) );
  race_e race = util::translate_race_id( profile[ "race" ].GetUint() );
  const module_t* module = module_t::get( class_name );

  if ( ! module || ! module -> valid() )
  {
    sim -> errorf( "\nModule for class %s is currently not available.\n", class_name.c_str() );
    return 0;
  }

  std::string name = player.name;

  if ( player.talent_spec != "active" && ! player.talent_spec.empty() )
  {
    name += '_';
    name += player.talent_spec;
  }

  if ( ! name.empty() )
    sim -> current_name = name;

  player_t* p = sim -> active_player = module -> create_player( sim, name, race );

  if ( ! p )
  {
    sim -> errorf( "BCP API: Unable to build player with class '%s' and name '%s' from '%s'.\n",
                   class_name.c_str(), name.c_str(), player.url.c_str() );
    return 0;
  }

  p -> level = profile[ "level" ].GetUint();
  p -> region_str = player.region.empty() ? sim -> default_region_str : player.region;
  if ( ! profile.HasMember( "realm" ) && ! player.server.empty() )
    p -> server_str = player.server;
  else
    p -> server_str = profile[ "realm" ].GetString();

  if ( ! player.origin.empty() )
    p -> origin_str = player.origin;

  if ( profile.HasMember( "thumbnail" ) )
    p -> report_information.thumbnail_url = "http://" + p -> region_str + ".battle.net/static-render/" +
                                            p -> region_str + '/' + profile[ "thumbnail" ].GetString();

  if ( profile.HasMember( "professions" ) )
  {
    parse_profession( p -> professions_str, profile[ "professions" ], 0 );
    parse_profession( p -> professions_str, profile[ "professions" ], 1 );
  }

  if ( ! parse_talents( p, profile[ "talents" ], player.talent_spec ) )
    return 0;

  if ( ! parse_glyphs( p, profile[ "talents" ], player.talent_spec ) )
    return 0;

  if ( profile.HasMember( "items" ) && ! parse_items( p, profile[ "items" ] ) )
    return 0;

  if ( ! p -> server_str.empty() )
    p -> armory_extensions( p -> region_str, p -> server_str, player.name, caching );

  return p;
}

// download_item_data =======================================================

bool download_item_data( item_t& item, cache::behavior_e caching )
{
  rapidjson::Document js;
  if ( ! download_id( js, item.sim -> default_region_str, item.parsed.data.id, caching ) || 
       js.HasParseError() )
  {
    if ( caching != cache::ONLY )
    {
      item.sim -> errorf( "BCP API: Player '%s' unable to download item id '%u' at slot %s.\n",
                          item.player -> name(), item.parsed.data.id, item.slot_name() );
    }
    return false;
  }

  if ( item.sim -> debug )
  {
    rapidjson::StringBuffer b;
    rapidjson::PrettyWriter< rapidjson::StringBuffer > writer( b );

    js.Accept( writer );
    item.sim -> out_debug.raw() << b.GetString();
  }

  try
  {
    if ( ! js.HasMember( "id" ) ) throw( "id" );
    if ( ! js.HasMember( "itemLevel" ) ) throw( "item level" );
    if ( ! js.HasMember( "quality" ) ) throw( "quality" );
    if ( ! js.HasMember( "inventoryType" ) ) throw( "inventory type" );
    if ( ! js.HasMember( "itemClass" ) ) throw( "item class" );
    if ( ! js.HasMember( "itemSubClass" ) ) throw( "item subclass" );
    if ( ! js.HasMember( "name" ) ) throw( "name" );

    item.parsed.data.id = js[ "id" ].GetUint();
    item.parsed.data.level = js[ "itemLevel" ].GetUint();
    item.parsed.data.quality = js[ "quality" ].GetUint();
    item.parsed.data.inventory_type = js[ "inventoryType" ].GetUint();
    item.parsed.data.item_class = js[ "itemClass" ].GetUint();
    item.parsed.data.item_subclass = js[ "itemSubClass" ].GetUint();
    item.name_str = js[ "name" ].GetString();

    util::tokenize( item.name_str );

    if ( js.HasMember( "icon" ) ) item.icon_str = js[ "icon" ].GetString();
    if ( js.HasMember( "requiredLevel" ) ) item.parsed.data.req_level = js[ "requiredLevel" ].GetUint();
    if ( js.HasMember( "requiredSkill" ) ) item.parsed.data.req_skill = js[ "requiredSkill" ].GetUint();
    if ( js.HasMember( "requiredSkillRank" ) ) item.parsed.data.req_skill_level = js[ "requiredSkillRank" ].GetUint();
    if ( js.HasMember( "itemBind" ) ) item.parsed.data.bind_type = js[ "itemBind" ].GetUint();

    if ( js.HasMember( "weaponInfo" ) )
    {
      const rapidjson::Value& weaponInfo = js[ "weaponInfo" ];
      if ( ! weaponInfo.HasMember( "dps" ) ) throw( "dps" );
      if ( ! weaponInfo.HasMember( "weaponSpeed" ) ) throw( "weapon speed" );
      if ( ! weaponInfo.HasMember( "damage" ) ) throw( "damage" );

      const rapidjson::Value& damage = weaponInfo[ "damage" ];
      if ( ! damage.HasMember( "exactMin" ) ) throw( "weapon minimum damage" );

      item.parsed.data.delay = static_cast< unsigned >( weaponInfo[ "weaponSpeed" ].GetDouble() * 1000.0 );
      item.parsed.data.dmg_range = 2 - 2 * damage[ "exactMin" ].GetDouble() / ( weaponInfo[ "dps" ].GetDouble() * weaponInfo[ "weaponSpeed" ].GetDouble() );
    }

    if ( js.HasMember( "allowableClasses" ) )
    {
      for ( rapidjson::SizeType i = 0, n = js[ "allowableClasses" ].Size(); i < n; ++i )
        item.parsed.data.class_mask |= ( 1 << ( js[ "allowableClasses" ][ i ].GetInt() - 1 ) );
    }
    else
      item.parsed.data.class_mask = -1;

    if ( js.HasMember( "allowableRaces" ) )
    {
      for ( rapidjson::SizeType i = 0, n = js[ "allowableRaces" ].Size(); i < n; ++i )
        item.parsed.data.race_mask |= ( 1 << ( js[ "allowableRaces" ][ i ].GetInt() - 1 ) );
    }
    else
      item.parsed.data.race_mask = -1;

    if ( js.HasMember( "bonusStats" ) )
    {
      for ( rapidjson::SizeType i = 0, n = js[ "bonusStats" ].Size(); i < n; ++i )
      {
        const rapidjson::Value& stat = js[ "bonusStats" ][ i ];
        if ( ! stat.HasMember( "stat" ) ) throw( "bonus stat" );
        if ( ! stat.HasMember( "amount" ) ) throw( "bonus stat amount" );

        item.parsed.data.stat_type_e[ i ] = stat[ "stat" ].GetInt();
        item.parsed.data.stat_val[ i ] =  stat[ "amount" ].GetInt();

        if ( js.HasMember( "weaponInfo" ) && 
             ( item.parsed.data.stat_type_e[ i ] == ITEM_MOD_INTELLECT ||
               item.parsed.data.stat_type_e[ i ] == ITEM_MOD_SPIRIT ||
               item.parsed.data.stat_type_e[ i ] == ITEM_MOD_SPELL_POWER ) )
          item.parsed.data.flags_2 |= ITEM_FLAG2_CASTER_WEAPON;
      }
    }

    if ( js.HasMember( "socketInfo" ) && js[ "socketInfo" ].HasMember( "sockets" ) )
    {
      const rapidjson::Value& sockets = js[ "socketInfo" ][ "sockets" ];

    for (rapidjson::SizeType i = 0, n = as<rapidjson::SizeType>( std::min(static_cast< size_t >(sockets.Size()), sizeof_array(item.parsed.data.socket_color))); i < n; ++i)
      {
        if ( ! sockets[ i ].HasMember( "type" ) )
          continue;

        std::string color = sockets[ i ][ "type" ].GetString();

        if ( color == "META" )
          item.parsed.data.socket_color[ i ] = SOCKET_COLOR_META;
        else if ( color == "RED" )
          item.parsed.data.socket_color[ i ] = SOCKET_COLOR_RED;
        else if ( color == "YELLOW" )
          item.parsed.data.socket_color[ i ] = SOCKET_COLOR_YELLOW;
        else if ( color == "BLUE" )
          item.parsed.data.socket_color[ i ] = SOCKET_COLOR_BLUE;
        else if ( color == "PRISMATIC" )
          item.parsed.data.socket_color[ i ] = SOCKET_COLOR_PRISMATIC;
        else if ( color == "COGWHEEL" )
          item.parsed.data.socket_color[ i ] = SOCKET_COLOR_COGWHEEL;
        else if ( color == "HYDRAULIC" )
          item.parsed.data.socket_color[ i ] = SOCKET_COLOR_HYDRAULIC;
      }

      if ( js[ "socketInfo" ].HasMember( "socketBonus" ) )
      {
        std::string socketBonus = js[ "socketInfo" ][ "socketBonus" ].GetString();
        std::string stat;
        util::fuzzy_stats( stat, socketBonus );
        std::vector<stat_pair_t> bonus = item_t::str_to_stat_pair( stat );
        item.parsed.socket_bonus_stats.insert( item.parsed.socket_bonus_stats.end(), bonus.begin(), bonus.end() );
      }
    }

    if ( js.HasMember( "itemSet" ) && js[ "itemSet" ].HasMember( "id" ) )
    {
      item.parsed.data.id_set = js[ "itemSet" ][ "id" ].GetUint();
    }

    if ( js.HasMember( "nameDescription" ) )
    {
      std::string nameDescription = js[ "nameDescription" ].GetString();

      if ( util::str_in_str_ci( nameDescription, "heroic" ) )
        item.parsed.data.type_flags |= RAID_TYPE_HEROIC;
      else if ( util::str_in_str_ci( nameDescription, "raid finder" ) )
        item.parsed.data.type_flags |= RAID_TYPE_LFR;
      else if ( util::str_in_str_ci( nameDescription, "mythic" ) )
        item.parsed.data.type_flags |= RAID_TYPE_MYTHIC;

      if ( util::str_in_str_ci( nameDescription, "warforged" ) )
        item.parsed.data.type_flags |= RAID_TYPE_WARFORGED;
    }

    if ( js.HasMember( "itemSpells" ) )
    {
      const rapidjson::Value& spells = js[ "itemSpells" ];
      size_t spell_idx = 0;
      for ( rapidjson::SizeType i = 0, n = spells.Size(); i < n && spell_idx < sizeof_array( item.parsed.data.id_spell ); ++i )
      {
        const rapidjson::Value& spell = spells[ i ];
        if ( ! spell.HasMember( "spellId" ) || ! spell.HasMember( "trigger" ) )
          continue;

        int spell_id = spell[ "spellId" ].GetInt();
        int trigger_type = -1;

        if ( util::str_compare_ci( spell[ "trigger" ].GetString(), "ON_EQUIP" ) )
          trigger_type = ITEM_SPELLTRIGGER_ON_EQUIP;
        else if ( util::str_compare_ci( spell[ "trigger" ].GetString(), "ON_USE" ) )
          trigger_type = ITEM_SPELLTRIGGER_ON_USE;
        else if ( util::str_compare_ci( spell[ "trigger" ].GetString(), "ON_PROC" ) )
          trigger_type = ITEM_SPELLTRIGGER_CHANCE_ON_HIT;

        if ( trigger_type != -1 && spell_id > 0 )
        {
          item.parsed.data.id_spell[ spell_idx ] = spell_id;
          item.parsed.data.trigger_spell[ spell_idx ] = trigger_type;
          spell_idx++;
        }
      }
    }
  }
  catch ( const char* fieldname )
  {
    std::string error_str;
    if ( js.HasMember( "reason" ) )
      error_str = js[ "reason" ].GetString();

    if ( caching != cache::ONLY )
      item.sim -> errorf( "BCP API: Player '%s' unable to parse item '%u' %s at slot '%s': %s\n",
                          item.player -> name(), item.parsed.data.id, fieldname, item.slot_name(), error_str.c_str() );
    return false;
  }

  return true;
}

// download_roster ==========================================================

bool download_roster( rapidjson::Document& d,
                      sim_t* sim,
                      const std::string& region,
                      const std::string& server,
                      const std::string& name,
                      cache::behavior_e  caching )
{
  std::string url = "http://" + region + ".battle.net/api/wow/guild/" + server + '/' +
                    name + "?fields=members";

  std::string result;
  if ( ! http::get( result, url, caching ) )
    return false;

  d.Parse< 0 >( result.c_str() );
  if ( d.HasParseError() )
  {
    sim -> errorf( "BCP API: Unable to parse guild from '%s': Parse error '%s' @ %lu\n",
      url.c_str(), d.GetParseError(), d.GetErrorOffset() );

    return false;
  }

  if ( sim -> debug )
  {
    rapidjson::StringBuffer b;
    rapidjson::PrettyWriter< rapidjson::StringBuffer > writer( b );

    d.Accept( writer );
    sim -> out_debug.raw() << b.GetString();
  }

  return true;
}

} // close anonymous namespace ==============================================

// bcp_api::download_player_html =============================================

player_t* bcp_api::download_player_html( sim_t*             sim,
                                         const std::string& region,
                                         const std::string& server,
                                         const std::string& name,
                                         const std::string& talents,
                                         cache::behavior_e  caching )
{
  sim -> current_name = name;

  player_spec_t player;

  std::string battlenet = "http://" + region + ".battle.net/";

  player.url = battlenet + "wow/en/character/" + server + '/' + name + "/advanced";
  player.origin = battlenet + "wow/en/character/" + server + '/' + name + "/advanced";

  player.region = region;
  player.server = server;
  player.name = name;

  player.talent_spec = talents;

  return parse_player_html( sim, player, caching );
}

// bcp_api::download_player =================================================

player_t* bcp_api::download_player( sim_t*             sim,
                                    const std::string& region,
                                    const std::string& server,
                                    const std::string& name,
                                    const std::string& talents,
                                    cache::behavior_e  caching )
{
  sim -> current_name = name;

  player_spec_t player;

  std::string battlenet = "http://" + region + ".battle.net/";

  player.url = battlenet + "api/wow/character/" +
               server + '/' + name + "?fields=talents,items,professions&locale=en_US";
  player.origin = battlenet + "wow/en/character/" + server + '/' + name + "/advanced";

  player.region = region;
  player.server = server;
  player.name = name;

  player.talent_spec = talents;

  return parse_player( sim, player, caching );
}

// bcp_api::from_local_json =================================================

player_t* bcp_api::from_local_json( sim_t*             sim,
                                    const std::string& name,
                                    const std::string& file_path,
                                    const std::string& talents
                                  )
{
  sim -> current_name = name;

  player_spec_t player;

  player.local_json = file_path;
  player.origin = file_path;

  player.name = name;
  player.talent_spec = talents;

  return parse_player( sim, player, cache::ANY );
}

// bcp_api::download_item() =================================================

bool bcp_api::download_item( item_t& item, cache::behavior_e caching )
{
  bool ret = download_item_data( item, caching );
  if ( ret )
    item.source_str = "Blizzard";
  return ret;
}

// bcp_api::download_guild ==================================================

bool bcp_api::download_guild( sim_t* sim,
                              const std::string& region,
                              const std::string& server,
                              const std::string& name,
                              const std::vector<int>& ranks,
                              int player_filter,
                              int max_rank,
                              cache::behavior_e caching )
{
  rapidjson::Document js;

  if ( ! download_roster( js, sim, region, server, name, caching ) )
    return false;

  if ( ! js.HasMember( "members" ) )
  {
    sim -> errorf( "No members in guild %s,%s,%s", region.c_str(), server.c_str(), name.c_str() );
    return false;
  }

  std::vector<std::string> names;

  for ( rapidjson::SizeType i = 0, end = js[ "members" ].Size(); i < end; i++ )
  {
    const rapidjson::Value& member = js[ "members" ][ i ];
    if ( ! member.HasMember( "character" ) )
      continue;

    if ( ! member.HasMember( "rank" ) )
      continue;

    int rank = member[ "rank" ].GetInt();
    if ( ( max_rank > 0 && rank > max_rank ) || 
         ( ! ranks.empty() && range::find( ranks, rank ) == ranks.end() ) )
      continue;

    const rapidjson::Value& character = member[ "character" ];

    if ( ! character.HasMember( "level" ) || character[ "level" ].GetUint() < 85 )
      continue;

    if ( ! character.HasMember( "class" ) )
      continue;

    if ( ! character.HasMember( "name" ) )
      continue;

    if ( player_filter != PLAYER_NONE &&
         player_filter != util::translate_class_id( character[ "class" ].GetInt() ) )
      continue;

    names.push_back( character[ "name" ].GetString() );
  }

  if ( names.empty() ) return true;

  range::sort( names );

  for ( std::size_t i = 0, n = names.size(); i < n; ++i )
  {
    const std::string& cname = names[ i ];
    std::cout << "Downloading character: " << cname << std::endl;
    download_player( sim, region, server, cname, "active", caching );
  }

  return true;
}

// bcp_api::download_glyph ==================================================

bool bcp_api::download_glyph( player_t*          player,
                              std::string&       glyph_name,
                              const std::string& glyph_id,
                              cache::behavior_e  caching )
{
  const std::string& region =
    ( player -> region_str.empty() ? player -> sim -> default_region_str : player -> region_str );

  unsigned glyphid = strtoul( glyph_id.c_str(), 0, 10 );
  rapidjson::Document js;
  if ( ! download_id( js, region, glyphid, caching ) || js.HasParseError() || ! js.HasMember( "name" ) )
  {
    if ( caching != cache::ONLY )
      player -> sim -> errorf( "BCP API: Unable to download glyph id '%s'\n", glyph_id.c_str() );
    return false;
  }

  glyph_name = js[ "name" ].GetString();

  return true;
}
