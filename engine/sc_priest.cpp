// ==========================================================================
// Dedmonwakeen's Raid DPS/TPS Simulator.
// Send questions to natehieter@gmail.com
// ==========================================================================

#include "simulationcraft.h"

// ==========================================================================
// Priest
// ==========================================================================

struct priest_t : public player_t
{
  // Buffs

  // Discipline
  buff_t* buffs_borrowed_time;
  buff_t* buffs_dark_evangelism;
  buff_t* buffs_holy_evangelism;
  buff_t* buffs_dark_archangel;
  buff_t* buffs_holy_archangel;
  buff_t* buffs_inner_fire;
  buff_t* buffs_inner_focus;
  buff_t* buffs_inner_will;
  buff_t* buffs_weakened_soul;

  // Holy
  buff_t* buffs_chakra_pre;
  buff_t* buffs_chakra_chastise;
  buff_t* buffs_chakra_sanctuary;
  buff_t* buffs_chakra_serenity;
  buff_t* buffs_serendipity;
  buff_t* buffs_serenity;
  buff_t* buffs_surge_of_light;

  // Shadow
  buff_t* buffs_empowered_shadow;
  buff_t* buffs_glyph_of_shadow_word_death;
  buff_t* buffs_mind_melt;
  buff_t* buffs_mind_spike;
  buff_t* buffs_shadow_form;
  buff_t* buffs_shadow_orb;
  buff_t* buffs_shadowfiend;
  buff_t* buffs_spirit_tap;
  buff_t* buffs_vampiric_embrace;

  // Set Bonus
  buff_t* buffs_indulgence_of_the_penitent;
  buff_t* buffs_divine_fire;

  // Talents

  struct talents_list_t
  {
    // Discipline
    talent_t* improved_power_word_shield;
    talent_t* twin_disciplines;
    talent_t* mental_agility;
    talent_t* evangelism;
    talent_t* archangel;
    talent_t* inner_sanctum;
    talent_t* soul_warding;
    talent_t* renewed_hope;
    talent_t* power_infusion;
    talent_t* atonement;
    talent_t* inner_focus;
    talent_t* rapture;
    talent_t* borrowed_time;
    talent_t* strength_of_soul;
    talent_t* divine_aegis;
    talent_t* train_of_thought;
    talent_t* grace;
    talent_t* power_word_barrier;

    // Holy
    talent_t* improved_renew;
    talent_t* empowered_healing;
    talent_t* divine_fury;
    talent_t* surge_of_light;
    talent_t* inspiration;
    talent_t* divine_touch;
    talent_t* holy_concentration;
    talent_t* lightwell;
    talent_t* tome_of_light;
    talent_t* rapid_renewal;
    talent_t* serendipity;
    talent_t* body_and_soul;
    talent_t* chakra;
    talent_t* revelations;
    talent_t* test_of_faith;
    talent_t* state_of_mind;
    talent_t* circle_of_healing;

    // Shadow
    talent_t* darkness;
    talent_t* improved_devouring_plague;
    talent_t* improved_mind_blast;
    talent_t* mind_melt;
    talent_t* dispersion;
    talent_t* improved_shadow_word_pain;
    talent_t* pain_and_suffering;
    talent_t* masochism;
    talent_t* shadow_form;
    talent_t* twisted_faith;
    talent_t* veiled_shadows;
    talent_t* harnessed_shadows;
    talent_t* shadowy_apparition;
    talent_t* vampiric_embrace;
    talent_t* vampiric_touch;
    talent_t* sin_and_punishment;
    talent_t* phantasm;
    talent_t* paralysis;
  };
  talents_list_t talents;


  struct passive_spells_t
  {
    // Discipline
    passive_spell_t* enlightenment;
    passive_spell_t* meditation_disc;
    passive_spell_t* dark_evangelism_1;
    passive_spell_t* dark_evangelism_2;
    passive_spell_t* holy_evangelism_1;
    passive_spell_t* holy_evangelism_2;

    // Holy
    passive_spell_t* spiritual_healing;
    passive_spell_t* meditation_holy;

    // Shadow
    passive_spell_t* shadow_power;
    passive_spell_t* shadow_orbs;
    passive_spell_t* shadowy_apparition_num;
  };
  passive_spells_t passive_spells;


  struct mastery_spells_t
  {
    mastery_t* shield_discipline;
    mastery_t* echo_of_light;
    mastery_t* shadow_orb_power;
  };
  mastery_spells_t mastery_spells;


  struct active_spells_t
  {
    active_spell_t* mind_spike;
    active_spell_t* shadow_fiend;
    active_spell_t* holy_archangel;
    active_spell_t* holy_archangel2;
    active_spell_t* dark_archangel;
  };
  active_spells_t   active_spells;

  spell_data_t*     dark_flames;

  // Cooldowns
  cooldown_t*       cooldowns_mind_blast;
  cooldown_t*       cooldowns_shadow_fiend;
  cooldown_t*       cooldowns_chakra;
  cooldown_t*       cooldowns_rapture;
  cooldown_t*       cooldowns_inner_focus;
  cooldown_t*       cooldowns_penance;

  // DoTs
  dot_t*            dots_shadow_word_pain;
  dot_t*            dots_vampiric_touch;
  dot_t*            dots_devouring_plague;
  dot_t*            dots_holy_fire;
  dot_t*            dots_renew;

  // Gains
  gain_t* gains_dispersion;
  gain_t* gains_shadow_fiend;
  gain_t* gains_archangel;
  gain_t* gains_masochism;
  gain_t* gains_rapture;
  gain_t* gains_hymn_of_hope;
  gain_t* gains_divine_fire;

  // Uptimes
  uptime_t* uptimes_mind_spike[ 4 ];
  uptime_t* uptimes_dark_flames;
  uptime_t* uptimes_dark_evangelism[ 6 ];
  uptime_t* uptimes_holy_evangelism[ 6 ];
  uptime_t* uptimes_shadow_orb[ 4 ];
  uptime_t* uptimes_test_of_faith;

  // Procs
  proc_t* procs_shadowy_apparation;
  proc_t* procs_surge_of_light;

  // Special
  std::queue<spell_t* > shadowy_apparition_free_list;
  std::list<spell_t* >  shadowy_apparition_active_list;
  heal_t* heals_echo_of_light;
  bool echo_of_light_merged;
  spell_t* atonement_nc;
  spell_t* atonement_c;

  // Random Number Generators
  rng_t* rng_pain_and_suffering;

  // Options
  std::string power_infusion_target_str;

  // Mana Resource Tracker
  struct mana_resource_t
  {
    double mana_gain;
    double mana_loss;
    void reset() { memset( ( void* ) this, 0x00, sizeof( mana_resource_t ) ); }

    mana_resource_t() { reset(); }
  };

  mana_resource_t mana_resource;
  double max_mana_cost;
  std::vector<player_t *> party_list;

  // Glyphs
  struct glyphs_t
  {
    glyph_t* dispersion;
    glyph_t* divine_accuracy;
    glyph_t* holy_nova;
    glyph_t* inner_fire;
    glyph_t* mind_flay;
    glyph_t* penance;
    glyph_t* spirit_tap;
    glyph_t* shadow_word_death;
    glyph_t* shadow_word_pain;
    glyph_t* smite;
    glyph_t* renew;
    glyph_t* power_word_shield;
    glyph_t* prayer_of_healing;
    glyph_t* lightwell;
    glyph_t* circle_of_healing;
    glyph_t* prayer_of_mending;
  };
  glyphs_t glyphs;

  // Constants
  struct constants_t
  {
    double meditation_value;

    // Discipline
    double twin_disciplines_value;
    double dark_evangelism_damage_value;
    double holy_evangelism_damage_value;
    double holy_evangelism_mana_value;
    double dark_archangel_damage_value;
    double dark_archangel_mana_value;
    double holy_archangel_value;
    double archangel_mana_value;
    double inner_will_value;

    // Holy
    double holy_concentration_value;

    // Shadow
    double shadow_power_damage_value;
    double shadow_power_crit_value;
    double shadow_orb_proc_value;
    double shadow_orb_damage_value;
    double shadow_orb_mastery_value;

    double darkness_value;
    double improved_shadow_word_pain_value;
    double twisted_faith_static_value;
    double twisted_faith_dynamic_value;
    double shadow_form_value;
    double harnessed_shadows_value;
    double pain_and_suffering_value;

    double devouring_plague_health_mod;

    uint32_t max_shadowy_apparitions;

    constants_t() { memset( ( void * ) this, 0x0, sizeof( constants_t ) ); }
  };
  constants_t constants;

  // Power Mods
  struct power_mod_t
  {
    double devouring_plague;
    double mind_blast;
    double mind_flay;
    double mind_spike;
    double shadow_word_death;
    double shadow_word_pain;
    double vampiric_touch;
    double holy_fire;
    double smite;

    power_mod_t() { memset( ( void * ) this, 0x0, sizeof( power_mod_t ) ); }
  };
  power_mod_t power_mod;

  bool   use_shadow_word_death;
  int    use_mind_blast;
  int    recast_mind_blast;
  bool   was_sub_25;

  priest_t( sim_t* sim, const std::string& name, race_type r = RACE_NONE ) : player_t( sim, PRIEST, name, r )
  {
    if ( race == RACE_NONE ) race = RACE_NIGHT_ELF;

    tree_type[ PRIEST_DISCIPLINE ] = TREE_DISCIPLINE;
    tree_type[ PRIEST_HOLY       ] = TREE_HOLY;
    tree_type[ PRIEST_SHADOW     ] = TREE_SHADOW;

    use_shadow_word_death                = false;
    was_sub_25                           = false;
    echo_of_light_merged                 = false;

    use_mind_blast                       = 1;
    recast_mind_blast                    = 0;
    heals_echo_of_light                  = 0;
    atonement_nc                         = 0;
    atonement_c                          = 0;

    distance                             = 40;

    max_mana_cost                        = 0.0;

    dots_shadow_word_pain                = get_dot( "shadow_word_pain" );
    dots_vampiric_touch                  = get_dot( "vampiric_touch" );
    dots_devouring_plague                = get_dot( "devouring_plague" );
    dots_holy_fire                       = get_dot( "holy_fire" );
    dots_renew                           = get_dot( "renew" );

    cooldowns_mind_blast                 = get_cooldown( "mind_blast" );
    cooldowns_shadow_fiend               = get_cooldown( "shadow_fiend" );
    cooldowns_chakra                     = get_cooldown( "chakra"   );
    cooldowns_rapture                    = get_cooldown( "rapture" );
    cooldowns_rapture -> duration = 12.0;
    cooldowns_inner_focus                = get_cooldown( "inner_focus" );
    cooldowns_penance                    = get_cooldown( "penance" );

    create_talents();
    create_glyphs();
    create_options();
  }

  // Character Definition
  virtual void      init_base();
  virtual void      init_gains();
  virtual void      init_uptimes();
  virtual void      init_rng();
  virtual void      init_talents();
  virtual void      init_spells();
  virtual void      init_buffs();
  virtual void      init_values();
  virtual void      init_actions();
  virtual void      init_procs();
  virtual void      init_scaling();
  virtual void      reset();
  virtual void      init_party();
  virtual void      create_options();
  virtual bool      create_profile( std::string& profile_str, int save_type=SAVE_ALL, bool save_html=false );
  virtual action_t* create_action( const std::string& name, const std::string& options );
  virtual pet_t*    create_pet   ( const std::string& name, const std::string& type = std::string() );
  virtual void      create_pets();
  virtual void      copy_from( player_t* source );
  virtual int       decode_set( item_t& item );
  virtual int       primary_resource() SC_CONST { return RESOURCE_MANA; }
  virtual int       primary_role() SC_CONST;
  virtual double    composite_armor() SC_CONST;
  virtual double    composite_spell_power( const school_type school ) SC_CONST;
  virtual double    composite_spell_hit() SC_CONST;
  virtual double    composite_spell_haste() SC_CONST;
  virtual double    composite_player_multiplier( const school_type school ) SC_CONST;
  virtual double    composite_player_td_multiplier( const school_type school ) SC_CONST;
  virtual double    composite_player_heal_multiplier( const school_type school ) SC_CONST;
  virtual double    composite_movement_speed() SC_CONST;

  virtual double    matching_gear_multiplier( const attribute_type attr ) SC_CONST;

  virtual double    spirit() SC_CONST;

  virtual void      regen( double periodicity );
  virtual action_expr_t* create_expression( action_t*, const std::string& name );

  virtual double    resource_gain( int resource, double amount, gain_t* source=0, action_t* action=0 );
  virtual double    resource_loss( int resource, double amount, action_t* action=0 );

  virtual double    target_mitigation( double amount, const school_type school, int type, int result, action_t* a=0 );

  virtual double    empowered_shadows_amount() SC_CONST;
  virtual double    shadow_orb_amount() SC_CONST;
};

namespace   // ANONYMOUS NAMESPACE ==========================================
{

// ==========================================================================
// Priest Spell
// ==========================================================================

struct priest_spell_t : public spell_t
{
  void _init_priest_spell_t()
  {
    priest_t* p = player -> cast_priest();

    may_crit          = true;
    tick_may_crit     = true;

    dot_behavior      = DOT_REFRESH;
    weapon_multiplier = 0.0;

    crit_bonus_multiplier *= 1.0 + p -> constants.shadow_power_crit_value;
  }

  priest_spell_t( const char* n, player_t* player, const school_type s, int t ) :
    spell_t( n, player, RESOURCE_MANA, s, t )
  {
    _init_priest_spell_t();
  }

  priest_spell_t( const active_spell_t& s, int t = TREE_NONE ) :
    spell_t( s )
  {
    _init_priest_spell_t();
  }

  priest_spell_t( const char* n, player_t* player, const char* sname, int t = TREE_NONE ) :
    spell_t( n, sname, player, t )
  {
    _init_priest_spell_t();
  }

  priest_spell_t( const char* n, player_t* player, const uint32_t id, int t = TREE_NONE ) :
    spell_t( n, id, player, t )
  {
    _init_priest_spell_t();
  }

  virtual void player_buff()
  {
    spell_t::player_buff();

    priest_t* p = player -> cast_priest();

    for ( int i=0; i < 6; i++ )
    {
      p -> uptimes_dark_evangelism[ i ] -> update( i == p -> buffs_dark_evangelism -> stack() );
      p -> uptimes_holy_evangelism[ i ] -> update( i == p -> buffs_holy_evangelism -> stack() );
    }
  }

  virtual void assess_damage( player_t* t,
                              double amount,
                              int    dmg_type,
                              int travel_result )
  {
    priest_t* p = player -> cast_priest();

    spell_t::assess_damage( t, amount, dmg_type, travel_result );

    if ( p -> buffs_vampiric_embrace -> up() && result_is_hit( travel_result ) )
    {
      double a = amount * ( 1.0 + p -> constants.twin_disciplines_value );
      p -> resource_gain( RESOURCE_HEALTH, a * 0.06, p -> gains.vampiric_embrace );

      pet_t* r = p -> pet_list;

      while ( r )
      {
        r -> resource_gain( RESOURCE_HEALTH, a * 0.03, r -> gains.vampiric_embrace );
        r = r -> next_pet;
      }

      int num_players = ( int ) p -> party_list.size();

      for ( int i=0; i < num_players; i++ )
      {
        player_t* q = p -> party_list[ i ];

        q -> resource_gain( RESOURCE_HEALTH, a * 0.03, q -> gains.vampiric_embrace );

        r = q -> pet_list;

        while ( r )
        {
          r -> resource_gain( RESOURCE_HEALTH, a * 0.03, r -> gains.vampiric_embrace );
          r = r -> next_pet;
        }
      }
    }
  }

  static void trigger_atonement( player_t* player, int result, double damage )
  {
    priest_t* p = player -> cast_priest();

    // Atonement
    if ( p -> atonement_nc && p -> atonement_c )
    {
      double atonement_dmg = damage * p -> talents.atonement -> effect1().percent();
      if ( result == RESULT_CRIT )
      {
        // Assuming there is no cap for crits, since it's not clear how much it would be.
        p -> atonement_c -> base_dd_min = atonement_dmg;
        p -> atonement_c -> base_dd_max = atonement_dmg;
        p -> atonement_c -> round_base_dmg = false;
        p -> atonement_c -> execute();
      }
      else
      {
        // Hotfix Cap from 05.01.2011
        if ( atonement_dmg > p -> resource_max[ RESOURCE_HEALTH ] * 0.3 )
        {
          atonement_dmg = p -> resource_max[ RESOURCE_HEALTH ] * 0.3;
        }

        p -> atonement_nc -> base_dd_min = atonement_dmg;
        p -> atonement_nc -> base_dd_max = atonement_dmg;
        p -> atonement_nc -> round_base_dmg = false;
        p -> atonement_nc -> execute();
      }
    }
  }


  static void    trigger_shadowy_apparition( player_t* player );
  static void    add_more_shadowy_apparitions( player_t* player );
};

// ==========================================================================
// Priest Absorb
// ==========================================================================

struct priest_absorb_t : public absorb_t
{
  priest_absorb_t( const char* n, player_t* player, const char* sname, int t = TREE_NONE ) :
    absorb_t( n, player, sname, t )
  {
    may_crit          = false;
    tick_may_crit     = false;
    dot_behavior      = DOT_REFRESH;
    weapon_multiplier = 0.0;

    may_miss=may_resist=false;
  }

  priest_absorb_t( const char* n, player_t* player, const uint32_t id, int t = TREE_NONE ) :
    absorb_t( n, player, id, t )
  {
    may_crit          = false;
    tick_may_crit     = false;
    may_miss=may_resist=false;

    dot_behavior      = DOT_REFRESH;
    weapon_multiplier = 0.0;
  }

  virtual void player_buff()
  {
    absorb_t::player_buff();

    priest_t* p = player -> cast_priest();

    player_multiplier *= 1.0 + (  p -> composite_mastery() * p -> mastery_spells.shield_discipline -> ok() * 2.5 / 100.0 );
  }

  virtual double haste() SC_CONST
  {
    double h = absorb_t::haste();

    priest_t* p = player -> cast_priest();

    if ( p -> buffs_borrowed_time -> up() )
    {
      h *= 1.0 / ( 1.0 + p -> talents.borrowed_time -> effect1().percent() );
    }

    return h;
  }

  virtual void execute()
  {
    absorb_t::execute();

    priest_t* p = player -> cast_priest();

    if ( execute_time() > 0 && p -> buffs_borrowed_time -> up() )
      p -> buffs_borrowed_time -> expire();
  }
};

// ==========================================================================
// Priest Heal
// ==========================================================================

struct priest_heal_t : public heal_t
{
  struct divine_aegis_t : public priest_absorb_t
  {
    divine_aegis_t( player_t* player ) :
      priest_absorb_t( "divine_aegis", player, 47753 )
    {
      proc             = true;
      background       = true;
      direct_power_mod = 0;
    }
  };

  divine_aegis_t* da;

  void trigger_echo_of_light( heal_t* a, player_t* t )
  {
    priest_t* p = a -> player -> cast_priest();

    if ( ! p -> mastery_spells.echo_of_light -> ok() ) return;

    struct echo_of_light_t : public priest_heal_t
    {
      echo_of_light_t( player_t* p ) :
        priest_heal_t( "echo_of_light", p, 77485 )
      {
        base_tick_time = 1.0;
        num_ticks      = 6;

        background     = true;
        tick_may_crit  = false;
        hasted_ticks   = false;
        may_crit       = false;

        init();
      }
    };

    if ( ! p -> heals_echo_of_light ) p -> heals_echo_of_light = new echo_of_light_t( p );

    if ( p -> heals_echo_of_light -> dot -> ticking )
    {
      if ( p -> echo_of_light_merged )
      {
        // The old tick_dmg is multiplied by the remaining ticks, added to the new complete heal, and then again divided by num_ticks
        p -> heals_echo_of_light -> base_td = ( p -> heals_echo_of_light -> base_td *
                                                p -> heals_echo_of_light -> dot -> ticks() +
                                                a -> direct_dmg * p -> composite_mastery() *
                                                p -> mastery_spells.echo_of_light -> effect_base_value( 2 ) / 10000.0 ) /
                                              p -> heals_echo_of_light -> num_ticks;
        p -> heals_echo_of_light -> refresh_duration();
      }
      else
      {
        // The old tick_dmg is multiplied by the remaining ticks minus one!, added to the new complete heal, and then again divided by num_ticks
        p -> heals_echo_of_light -> base_td = ( p -> heals_echo_of_light -> base_td *
                                                ( p -> heals_echo_of_light -> dot -> ticks() - 1 ) +
                                                a -> direct_dmg * p -> composite_mastery() *
                                                p -> mastery_spells.echo_of_light -> effect_base_value( 2 ) / 10000.0 ) /
                                              p -> heals_echo_of_light -> num_ticks;
        p -> heals_echo_of_light -> refresh_duration();
        p -> echo_of_light_merged = true;
      }
    }
    else
    {
      p -> heals_echo_of_light -> base_td = a -> direct_dmg * p -> composite_mastery() *
                                            p -> mastery_spells.echo_of_light -> effect_base_value( 2 ) / 10000.0 /
                                            p -> heals_echo_of_light -> num_ticks;
      p -> heals_echo_of_light -> execute();
      p -> echo_of_light_merged = false;
    }
  }


  priest_heal_t( const char* n, player_t* player, const char* sname, int t = TREE_NONE ) :
    heal_t( n, player, sname, t ), da( 0 )
  {
    priest_t* p = player -> cast_priest();

    if ( p -> talents.divine_aegis -> ok() )
    {
      da = new divine_aegis_t( p );
    }
  }

  priest_heal_t( const char* n, player_t* player, const uint32_t id, int t = TREE_NONE ) :
    heal_t( n, player, id, t ), da( 0 )
  {
    priest_t* p = player -> cast_priest();

    if ( p -> talents.divine_aegis -> ok() )
    {
      da = new divine_aegis_t( p );
    }
  }

  virtual void player_buff()
  {
    heal_t::player_buff();

    priest_t* p = player -> cast_priest();

    player_multiplier *= 1.0 + p -> passive_spells.spiritual_healing -> base_value( E_APPLY_AURA, A_ADD_PCT_MODIFIER ) ;

    if ( p -> buffs_serenity -> up() )
      player_crit += 0.10;

    player_multiplier *= 1.0 + p -> buffs_holy_archangel -> stack() * 0.03;
  }

  virtual void target_debuff( player_t* t, int dmg_type )
  {
    heal_t::target_debuff( t, dmg_type );

    priest_t* p = player -> cast_priest();


    // Grace
    if ( p -> talents.grace -> ok() )
      target_multiplier *= 1.0 + t -> buffs.grace -> value();

    // Test of Faith
    if ( p -> talents.test_of_faith -> rank() )
    {
      if (  t -> resource_current[ RESOURCE_HEALTH ] * 2 <= t -> resource_max[ RESOURCE_HEALTH ] )
      {
        target_multiplier *= 1.0 + p -> talents.test_of_faith -> rank() * 0.04;
        p -> uptimes_test_of_faith -> update( true );
      }
      else
        p -> uptimes_test_of_faith -> update( false );
    }
  }

  virtual double haste() SC_CONST
  {
    double h = heal_t::haste();

    priest_t* p = player -> cast_priest();

    if ( p -> buffs_borrowed_time -> up() )
      h *= 1.0 / ( 1.0 + p -> talents.borrowed_time -> effect1().percent() );

    return h;
  }

  virtual void execute()
  {
    heal_t::execute();

    priest_t* p = player -> cast_priest();

    if ( execute_time() > 0 && p -> buffs_borrowed_time -> up() )
      p -> buffs_borrowed_time -> expire();
  }

  virtual void travel( player_t* t, int travel_result, double travel_dmg )
  {
    heal_t::travel( t, travel_result, travel_dmg );
    priest_t* p = player -> cast_priest();

    // Divine Aegis
    if ( da && travel_result == RESULT_CRIT )
    {
      da -> base_dd_min = travel_dmg * 0.3;
      da -> base_dd_max = travel_dmg * 0.3;
      da -> heal_target.clear();
      da -> heal_target.push_back( t );
      da -> execute();
    }

    if ( travel_dmg > 0 )
    {
      trigger_echo_of_light( this, t );
      if ( p -> buffs_chakra_serenity -> up() && p -> dots_renew -> ticking )
        p -> dots_renew -> action -> refresh_duration();
    }
  }

  void trigger_inspiration( int insp_result, player_t* t )
  {
    priest_t* p = player -> cast_priest();

    if ( p -> talents.inspiration -> rank() && insp_result == RESULT_CRIT )
      t -> buffs.inspiration -> trigger( 1, p -> talents.inspiration -> rank() * 5 );
  }

  void trigger_grace( player_t* t )
  {
    priest_t* p = player -> cast_priest();

    if ( p -> talents.grace -> rank() )
      t -> buffs.grace -> trigger( 1, p -> dbc.effect( p -> dbc.spell( p -> talents.grace -> effect_trigger_spell( 1 ) ) -> effect1().id() ) -> base_value() / 100.0 );
  }

  void trigger_strength_of_soul( player_t* t )
  {
    priest_t* p = player -> cast_priest();

    if ( p -> talents.strength_of_soul -> rank() && p -> buffs_weakened_soul -> up() )
      p -> buffs_weakened_soul -> extend_duration( p, -1 * p -> talents.strength_of_soul -> effect1().base_value() );
  }
};


// ==========================================================================
// Pet Shadow Fiend
// ==========================================================================

struct shadow_fiend_pet_t : public pet_t
{
  buff_t*   buffs_shadowcrawl;
  active_spell_t* shadowcrawl;
  passive_spell_t* mana_leech;
  bool bad_swing;
  double bad_spell_power;
  bool extra_tick;

  struct tier12_flame_attack_t : public spell_t
  {
    spell_data_t* shadowflame;
    double dmg_mult;

    tier12_flame_attack_t( player_t* player ) : 
      spell_t( "Shadowflame", player, SCHOOL_FIRE ), shadowflame( NULL ), dmg_mult( 0.2 )
    {
      priest_t* o = player -> cast_pet() -> owner -> cast_priest();

      background       = true;
      may_miss         = false;    
      proc             = true;      
      may_crit         = false;
      direct_power_mod = 0.0;
      trigger_gcd      = false;
      school           = SCHOOL_FIRE;

      shadowflame = spell_data_t::find( 99156, "Shadowflame", o -> dbc.ptr );
      if ( shadowflame )
      {
        dmg_mult = shadowflame->effect1().percent();
      }
    }
    
    virtual double calculate_direct_damage()
    {
      double dmg = base_dd_min * dmg_mult;

      if ( ! binary )
      {
        dmg *= 1.0 - resistance();
      }

      return dmg;
    }
  };

  struct shadowcrawl_t : public spell_t
  {
    shadowcrawl_t( player_t* player ) :
      spell_t( *( ( ( shadow_fiend_pet_t* ) ( player -> cast_pet() ) ) -> shadowcrawl ) )
    {
      may_miss = false;
    }

    virtual void execute()
    {
      spell_t::execute();

      shadow_fiend_pet_t* p = ( shadow_fiend_pet_t* ) player -> cast_pet();

      p -> buffs_shadowcrawl -> start( 1, p -> shadowcrawl -> effect_base_value( 2 ) / 100.0 );
    }
  };

  struct melee_t : public attack_t
  {
    tier12_flame_attack_t* tier12_flame_attack_spell;

    melee_t( player_t* player ) :
      attack_t( "melee", player, RESOURCE_NONE, SCHOOL_SHADOW ), tier12_flame_attack_spell( 0 )
    {
      priest_t* o = player -> cast_pet() -> owner -> cast_priest();
      weapon = &( player -> main_hand_weapon );
      base_execute_time = weapon -> swing_time;
      weapon_multiplier = 0;
      direct_power_mod = 0.0064 * o -> level;
      if ( harmful ) base_spell_power_multiplier = 1.0;
      base_attack_power_multiplier = 0.0;
      base_dd_min = util_t::ability_rank( player -> level,  221.0,85,  197.0,82,  175.0,80,  1.0,0 );
      base_dd_max = util_t::ability_rank( player -> level,  271.0,85,  245.0,82,  222.0,80,  2.0,0 );
      background = true;
      repeating  = true;
      may_dodge  = true;
      may_miss   = false;
      may_parry  = false; // Technically it can be parried on the first swing or if the rear isn't reachable
      may_crit   = true;
      may_block  = false; // Technically it can be blocked on the first swing or if the rear isn't reachable

      if ( o -> set_bonus.tier12_2pc_caster() )
      {
        tier12_flame_attack_spell = new tier12_flame_attack_t( player );

        add_child( tier12_flame_attack_spell );
      }
    }

    void assess_damage( player_t* t, double amount, int dmg_type, int travel_result )
    {
      attack_t::assess_damage( t, amount, dmg_type, travel_result );

      shadow_fiend_pet_t* p = ( shadow_fiend_pet_t* ) player -> cast_pet();
      priest_t* o = p -> owner -> cast_priest();

      if ( result_is_hit( travel_result ) )
        o -> resource_gain( RESOURCE_MANA, o -> resource_max[ RESOURCE_MANA ] *
                            p -> mana_leech -> effect_base_value( 1 ) / 100.0,
                            o -> gains_shadow_fiend );
    }

    void player_buff()
    {
      attack_t::player_buff();

      shadow_fiend_pet_t* p = ( shadow_fiend_pet_t* ) player -> cast_pet();

      if ( p -> bad_swing )
        p -> bad_swing = false;

      player_multiplier *= 1.0 + p -> buffs_shadowcrawl -> value();
    }

    void execute()
    {
      attack_t::execute();

      if ( tier12_flame_attack_spell )
      {
        tier12_flame_attack_spell -> base_dd_min = direct_dmg;
        tier12_flame_attack_spell -> execute();
      }
    }
  };

  shadow_fiend_pet_t( sim_t* sim, player_t* owner ) :
    pet_t( sim, owner, "shadow_fiend" ), buffs_shadowcrawl( 0 ), shadowcrawl( 0 ), mana_leech( 0 ),
    bad_swing( false ), bad_spell_power( 0.0 ), extra_tick( false )
  {
    main_hand_weapon.type       = WEAPON_BEAST;
    main_hand_weapon.swing_time = 1.5;
    main_hand_weapon.school     = SCHOOL_SHADOW;

    stamina_per_owner           = 0.30;
    intellect_per_owner         = 0.50;

    bad_spell_power = util_t::ability_rank( owner -> level,  370.0,85,  358.0,82,  352.0,80,  0.0,0 );

    action_list_str             = "/snapshot_stats/shadowcrawl/wait_until_ready";
  }

  virtual action_t* create_action( const std::string& name,
                                   const std::string& options_str )
  {
    if ( name == "shadowcrawl" ) return new shadowcrawl_t( this );

    return pet_t::create_action( name, options_str );
  }

  virtual void init_spells()
  {
    player_t::init_spells();

    shadowcrawl                 = new active_spell_t ( this, "shadowcrawl", "Shadowcrawl" );
    mana_leech                  = new passive_spell_t( this, "mana_leech", 34650 );
  }

  virtual void init_base()
  {
    pet_t::init_base();

    attribute_base[ ATTR_STRENGTH  ]  = 0; // Unknown
    attribute_base[ ATTR_AGILITY   ]  = 0; // Unknown
    attribute_base[ ATTR_STAMINA   ]  = 0; // Unknown
    attribute_base[ ATTR_INTELLECT ]  = 0; // Unknown
    resource_base[ RESOURCE_HEALTH ]  = util_t::ability_rank( owner -> level,  18480.0,85,  7475.0,82,  6747.0,80,  100.0,0 );
    resource_base[ RESOURCE_MANA   ]  = util_t::ability_rank( owner -> level,  16828.0,85,  9824.0,82,  7679.0,80,  100.0,0 );
    base_attack_power                 = 0;  // Unknown
    base_attack_crit                  = 0.07; // Needs more testing
    initial_attack_power_per_strength = 0; // Unknown

    main_hand_attack = new melee_t( this );
  }

  virtual void init_buffs()
  {
    pet_t::init_buffs();

    buffs_shadowcrawl = new buff_t( this, "shadowcrawl", 1, shadowcrawl->duration() );
  }

  virtual double composite_spell_power( const school_type school ) SC_CONST
  {
    priest_t* p = owner -> cast_priest();

    double sp;

    if ( bad_swing )
      sp = bad_spell_power;
    else
      sp = p -> composite_spell_power( school );

    return sp;
  }

  virtual double composite_attack_hit() SC_CONST
  {
    return owner -> composite_spell_hit();
  }

  virtual double composite_attack_expertise() SC_CONST
  {
    return owner -> composite_spell_hit() * 26.0 / 17.0;
  }

  virtual double composite_attack_crit() SC_CONST
  {
    double c = pet_t::composite_attack_crit();

    c += owner -> composite_spell_crit(); // Needs confirming that it benefits from ALL owner crit.

    return c;
  }

  virtual void summon( double duration )
  {
    priest_t* p = owner -> cast_priest();

    // Simulate "Bad" swings
    if ( owner -> bugs && owner -> sim -> roll( 0.3 ) )
    {
      bad_swing = true;
    }
    // Simulate extra tick
    if ( !bugs || !owner -> sim -> roll( 0.5 ) )
    {
      duration -= 0.1;
    }

    dismiss();

    pet_t::summon( duration );

    p -> buffs_shadowfiend -> start();
  }
  virtual void dismiss()
  {
    priest_t* p = owner -> cast_priest();

    pet_t::dismiss();

    p -> buffs_shadowfiend -> expire();
  }
  virtual void schedule_ready( double delta_time=0,
                               bool   waiting=false )
  {
    pet_t::schedule_ready( delta_time, waiting );
    if ( ! main_hand_attack -> execute_event ) main_hand_attack -> execute();
  }
};


// ==========================================================================
// Priest Spell Increments
// ==========================================================================



// Shadowy Apparation Spell ============================================================

struct shadowy_apparition_t : public priest_spell_t
{
  shadowy_apparition_t( player_t* player ) :
    priest_spell_t( "shadowy_apparition", player, 87532 )
  {
    background        = true;
    proc              = true;

    trigger_gcd       = 0;
    travel_speed      = 3.5;

    base_crit += 0.06; // estimated.

    if ( player -> bugs )
    {
      base_dd_min *= 1.0625;
      base_dd_max *= 1.0625;
    }

    init();
  }

  virtual void travel( player_t* t, int result, double dmg )
  {
    priest_t* p = player -> cast_priest();

    priest_spell_t::travel( t, result, dmg );

    // Cleanup. Re-add to free list.
    p -> shadowy_apparition_active_list.remove( this );
    p -> shadowy_apparition_free_list.push( this );
  }

  virtual void player_buff()
  {
    priest_t* p = player -> cast_priest();

    priest_spell_t::player_buff();

    if ( player -> bugs )
    {
      player_multiplier /= 1.0 + p -> constants.twisted_faith_static_value;
    }

    player_multiplier *= 1.0 + p -> sets -> set( SET_T11_4PC_CASTER ) -> mod_additive( P_GENERIC );
  }
};

void priest_spell_t::trigger_shadowy_apparition( player_t* player )
{
  priest_t* p = player -> cast_priest();

  spell_t* s = NULL;

  if ( p -> talents.shadowy_apparition -> rank() )
  {
    double h = p -> talents.shadowy_apparition -> rank() * ( p -> is_moving() ? 0.2 : 0.04 );
    if ( p -> sim -> roll( h ) && ( !p -> shadowy_apparition_free_list.empty() ) )
    {
      s = p -> shadowy_apparition_free_list.front();

      p -> shadowy_apparition_free_list.pop();

      p -> shadowy_apparition_active_list.push_back( s );

      p -> procs_shadowy_apparation -> occur();

      s -> execute();
    }
  }
}

void priest_spell_t::add_more_shadowy_apparitions( player_t* player )
{
  priest_t* p = player -> cast_priest();

  spell_t* s = NULL;

  if ( ! p -> shadowy_apparition_free_list.size() )
  {
    for ( uint32_t i = 0; i < p -> constants.max_shadowy_apparitions; i++ )
    {
      s = new shadowy_apparition_t( p );
      p -> shadowy_apparition_free_list.push( s );
    }
  }
}


// ==========================================================================
// Priest Actions
// ==========================================================================


// Devouring Plague Spell ===================================================

struct devouring_plague_burst_t : public priest_spell_t
{
  devouring_plague_burst_t( player_t* player ) :
    priest_spell_t( "devouring_plague_burst", player, SCHOOL_SHADOW, TREE_SHADOW )
  {
    proc       = true;
    background = true;
  }

  virtual void player_buff()
  {
    priest_spell_t::player_buff();

    priest_t* p = player -> cast_priest();

    double m = 1.0;

    m += p -> buffs_dark_evangelism -> stack () * p -> constants.dark_evangelism_damage_value;
    m += p -> buffs_empowered_shadow -> value();

    if ( ! p -> bugs )
    {
      m += 0.01 * p -> buffs.dark_intent_feedback -> stack();
    }

    player_multiplier *= m;
  }
};

struct devouring_plague_t : public priest_spell_t
{
  devouring_plague_burst_t* burst_spell;

  devouring_plague_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "devouring_plague", player, "Devouring Plague" ), burst_spell( 0 )
  {
    priest_t* p = player -> cast_priest();
    parse_options( NULL, options_str );

    may_crit   = false;

    base_cost        *= 1.0 + p -> talents.mental_agility -> mod_additive( P_RESOURCE_COST );
    base_cost         = floor( base_cost );

    if ( p -> talents.improved_devouring_plague -> rank() )
    {
      burst_spell = new devouring_plague_burst_t( p );

      add_child( burst_spell );
    }
  }

  virtual double cost() SC_CONST
  {
    double c = priest_spell_t::cost();

    priest_t* p = player -> cast_priest();

    c *= 1.0 - p -> buffs_inner_will -> value();
    c  = floor( c );

    return c;
  }

  virtual void execute()
  {
    priest_t* p = player -> cast_priest();

    priest_spell_t::execute();

    // Improved Devouring Plague
    if ( burst_spell )
    {
      double dmg = ceil( base_td ) + total_power() * tick_power_mod;

      dmg *= p -> talents.improved_devouring_plague -> base_value( E_APPLY_AURA, A_DUMMY, P_DAMAGE_TAKEN ) / 100.0;

      double n = hasted_num_ticks();

      if ( p -> bugs )
      {
        // Currently it's rounding up but only using haste rating haste.
        double d = num_ticks * base_tick_time;
        double t = floor( ( base_tick_time * p -> spell_haste * 1000.0 ) + 0.5 ) / 1000.0;
        n = ( int ) ceil( ( d / t ) );
      }

      burst_spell -> base_dd_min    = dmg * n;
      burst_spell -> base_dd_max    = dmg * n;
      burst_spell -> round_base_dmg = false;
      burst_spell -> execute();
    }
  }

  virtual void tick()
  {
    priest_spell_t::tick();

    priest_t* p = player -> cast_priest();

    player -> resource_gain( RESOURCE_HEALTH, tick_dmg * p -> constants.devouring_plague_health_mod );
  }
};

// Dispersion Spell =========================================================

struct dispersion_t : public priest_spell_t
{
  pet_t* shadow_fiend;

  dispersion_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "dispersion", player, "Dispersion" )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    check_talent( p -> talents.dispersion -> rank() );

    base_execute_time = 0.0;
    base_tick_time    = 1.0;
    num_ticks         = 6;

    channeled         = true;
    harmful           = false;
    tick_may_crit     = false;

    if ( p -> glyphs.dispersion -> ok() ) cooldown -> duration -= 45;

    shadow_fiend = p -> find_pet( "shadow_fiend" );
  }

  virtual void tick()
  {
    priest_t* p = player -> cast_priest();

    p -> resource_gain( RESOURCE_MANA, 0.06 * p -> resource_max[ RESOURCE_MANA ], p -> gains_dispersion );

    priest_spell_t::tick();
  }

  virtual bool ready()
  {
    if ( ! priest_spell_t::ready() )
      return false;

    priest_t* p = player -> cast_priest();

    if ( ! shadow_fiend -> sleeping ) return false;

    double sf_cooldown_remains  = p -> cooldowns_shadow_fiend -> remains();
    double sf_cooldown_duration = p -> cooldowns_shadow_fiend -> duration;

    if ( sf_cooldown_remains <= 0 ) return false;

    double     max_mana = p -> resource_max    [ RESOURCE_MANA ];
    double current_mana = p -> resource_current[ RESOURCE_MANA ];

    double consumption_rate = ( p -> mana_resource.mana_loss - p -> mana_resource.mana_gain ) / sim -> current_time;
    double time_to_die = sim -> target -> time_to_die();

    if ( consumption_rate <= 0.00001 ) return false;

    double oom_time = current_mana / consumption_rate;

    if ( oom_time >= time_to_die ) return false;

    double sf_regen = 0.30 * max_mana;

    consumption_rate -= sf_regen / sf_cooldown_duration;

    if ( consumption_rate <= 0.00001 ) return false;

    double future_mana = current_mana + sf_regen - consumption_rate * sf_cooldown_remains;

    if ( future_mana > max_mana ) future_mana = max_mana;

    oom_time = future_mana / consumption_rate;

    if ( oom_time >= time_to_die ) return false;

    return ( max_mana - current_mana ) > p -> max_mana_cost;
  }
};

// Fortitude Spell ==========================================================

struct fortitude_t : public priest_spell_t
{
  double bonus;

  fortitude_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "fortitude", player, "Power Word: Fortitude" ), bonus( 0 )
  {
    parse_options( NULL, options_str );

    harmful = false;

    bonus = floor( player -> dbc.effect_average( player -> dbc.spell( 79104 ) -> effect1().id() , player -> level ) );
  }

  virtual void execute()
  {
    priest_spell_t::execute();

    for ( player_t* p = sim -> player_list; p; p = p -> next )
    {
      if ( p -> ooc_buffs() )
      {
        double before = p -> attribute[ ATTR_STAMINA ];
        p -> buffs.fortitude -> trigger( 1, bonus );
        double  after = p -> attribute[ ATTR_STAMINA ];
        p -> stat_gain( STAT_HEALTH, ( after - before ) * p -> health_per_stamina );
      }
    }
  }

  virtual bool ready()
  {
    if ( player -> buffs.fortitude -> current_value >= bonus ||
         sim -> auras.qiraji_fortitude -> check() )
      return false;

    return priest_spell_t::ready();
  }
};

// Holy Fire Spell ==========================================================

struct holy_fire_t : public priest_spell_t
{
  holy_fire_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "holy_fire", player, "Holy Fire" )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    base_execute_time += p -> talents.divine_fury -> effect1().seconds();

    base_hit       += p -> glyphs.divine_accuracy -> ok() ? 0.18 : 0.0;
  }

  virtual void execute()
  {
    priest_spell_t::execute();

    priest_t* p = player -> cast_priest();

    p -> buffs_holy_evangelism  -> trigger();

    trigger_atonement( player, result, direct_dmg );
  }

  virtual void tick()
  {
    priest_spell_t::tick();

    trigger_atonement( player, result, tick_dmg );
  }

  virtual void player_buff()
  {
    priest_spell_t::player_buff();

    priest_t* p = player -> cast_priest();

    player_multiplier *= 1.0 + ( p -> talents.evangelism -> rank() * p -> buffs_holy_evangelism -> stack() * p -> constants.holy_evangelism_damage_value );
  }

  virtual double cost() SC_CONST
  {
    double c = priest_spell_t::cost();

    priest_t* p = player -> cast_priest();

    c *= 1.0 - ( p -> talents.evangelism -> rank() * p -> buffs_holy_evangelism -> check() * p -> constants.holy_evangelism_mana_value );

    return c;
  }
};

// Inner Fire Spell =========================================================

struct inner_fire_t : public priest_spell_t
{
  inner_fire_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "inner_fire", player, "Inner Fire" )
  {
    parse_options( NULL, options_str );

    harmful = false;
  }

  virtual void execute()
  {
    priest_t* p = player -> cast_priest();

    priest_spell_t::execute();

    p -> buffs_inner_will       -> expire ();

    p -> buffs_inner_fire       -> trigger();
  }

  virtual bool ready()
  {
    priest_t* p = player -> cast_priest();

    if ( p -> buffs_inner_fire -> check() )
      return false;

    return priest_spell_t::ready();
  }
};

// Inner Will Spell =========================================================

struct inner_will_t : public priest_spell_t
{
  double value;

  inner_will_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "inner_will", player, "Inner Will" ), value( 0.0 )
  {
    parse_options( NULL, options_str );

    harmful = false;

    value = mod_additive( P_RESOURCE_COST );
  }

  virtual void execute()
  {
    priest_t* p = player -> cast_priest();

    priest_spell_t::execute();

    p -> buffs_inner_fire -> expire();

    p -> buffs_inner_will -> trigger( 1, -value );
  }

  virtual bool ready()
  {
    priest_t* p = player -> cast_priest();

    if ( p -> buffs_inner_will -> check() )
      return false;

    return priest_spell_t::ready();
  }
};

// Mind Blast Spell =========================================================

struct mind_blast_t : public priest_spell_t
{
  stats_t* orb_stats[ 4 ];

  mind_blast_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "mind_blast", player, "Mind Blast" )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    cooldown -> duration -= p -> talents.improved_mind_blast -> rank() * 0.5;

    for( int i=0; i < 4; i++ )
    {
      std::string str = "mind_blast_";
      orb_stats[ i ] = p -> get_stats( str + ( char ) ( i + ( int ) '0' ), this );
    }
  }

  virtual void execute()
  {
    double saved_cooldown = cooldown -> duration;

    priest_t* p = player -> cast_priest();

    stats = orb_stats[ p -> buffs_shadow_orb -> stack() ];

    priest_spell_t::execute();

    cooldown -> duration = saved_cooldown;

    for ( int i=0; i < 4; i++ )
    {
      p -> uptimes_mind_spike[ i ] -> update( i == p -> buffs_mind_spike -> stack() );
    }
    for ( int i=0; i < 4; i++ )
    {
      p -> uptimes_shadow_orb[ i ] -> update( i == p -> buffs_shadow_orb -> stack() );
    }

    p -> buffs_mind_melt -> expire();
    p -> buffs_mind_spike -> expire();
    p -> buffs_shadow_orb -> expire();

    p -> buffs_empowered_shadow -> trigger( 1, p -> empowered_shadows_amount() );

    if ( result_is_hit() )
    {
      p -> recast_mind_blast = 0;

      if ( p -> dots_vampiric_touch -> ticking )
        p -> trigger_replenishment();
    }
  }

  virtual void player_buff()
  {
    priest_t* p = player -> cast_priest();
    double m = 1.0;

    priest_spell_t::player_buff();

    if ( p -> ptr && p -> set_bonus.tier12_4pc_caster() )
    {
      if ( p -> dots_shadow_word_pain -> ticking && p -> dots_vampiric_touch -> ticking && p -> dots_devouring_plague -> ticking )
      {
        m += p -> dark_flames ? p -> dark_flames -> effect1().percent() : 0.25;
        p -> uptimes_dark_flames -> update( 1 );
      }
      else
      {
        p -> uptimes_dark_flames -> update( 0 );
      }     
    }

    m += p -> buffs_dark_archangel -> value() * p -> constants.dark_archangel_damage_value;

    player_multiplier *= m;

    player_multiplier *= 1.0 + ( p -> buffs_shadow_orb -> stack() * p -> shadow_orb_amount() );
  }

  virtual void target_debuff( player_t* t, int dmg_type )
  {
    priest_spell_t::target_debuff( t, dmg_type );

    priest_t* p = player -> cast_priest();

    target_crit       += p -> buffs_mind_spike -> value() * p -> buffs_mind_spike -> check();
  }

  virtual double execute_time() SC_CONST
  {
    double a = priest_spell_t::execute_time();

    priest_t* p = player -> cast_priest();

    a *= 1 - ( p -> buffs_mind_melt -> check() * ( p -> talents.mind_melt -> rank() * 0.25 ) );

    return a;
  }
};

// Mind Flay Spell ==========================================================

struct mind_flay_t : public priest_spell_t
{
  double mb_wait;
  int    swp_refresh;
  int    cut_for_mb;

  mind_flay_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "mind_flay", player, "Mind Flay" ), mb_wait( 0 ), swp_refresh( 0 ), cut_for_mb( 0 )
  {
    priest_t* p = player -> cast_priest();

    check_spec( TREE_SHADOW );

    option_t options[] =
    {
      { "cut_for_mb",            OPT_BOOL, &cut_for_mb            },
      { "mb_wait",               OPT_FLT,  &mb_wait               },
      { "swp_refresh",           OPT_BOOL, &swp_refresh           },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    may_crit     = false;
    channeled    = true;
    hasted_ticks = false;

    base_crit      += p -> sets -> set( SET_T11_2PC_CASTER ) -> mod_additive( P_CRIT );
  }

  virtual void execute()
  {
    priest_t* p = player -> cast_priest();

    if ( cut_for_mb )
      if ( p -> get_cooldown( "mind_blast" ) -> remains() <= ( 2 * base_tick_time * haste() ) )
        num_ticks = 2;

    priest_spell_t::execute();

    if ( result_is_hit() )
    {
      // Evangelism procs off both the initial cast and each tick.
      p -> buffs_dark_evangelism  -> trigger( 1, 1.0, p -> talents.evangelism -> rank() ? 1.0 : 0.0 );
    }
  }

  virtual void player_buff()
  {
    priest_t* p = player -> cast_priest();

    priest_spell_t::player_buff();

    player_td_multiplier += p -> buffs_dark_archangel -> value() * p -> constants.dark_archangel_damage_value;

    if ( p -> glyphs.mind_flay -> ok() )
    {
      player_td_multiplier += 0.10;
    }
  }

  virtual void tick()
  {
    priest_spell_t::tick();

    priest_t* p = player -> cast_priest();

    if ( result_is_hit() )
    {
      p -> buffs_dark_evangelism  -> trigger( 1, 1.0, p -> talents.evangelism -> rank() ? 1.0 : 0.0 );
      p -> buffs_shadow_orb  -> trigger( 1, 1, p -> constants.shadow_orb_proc_value + p -> constants.harnessed_shadows_value );

      if ( p -> dots_shadow_word_pain -> ticking )
      {
        if ( p -> rng_pain_and_suffering -> roll( p -> constants.pain_and_suffering_value ) )
        {
          p -> dots_shadow_word_pain -> action -> refresh_duration();
        }
      }
      if ( result == RESULT_CRIT )
      {
        p -> cooldowns_shadow_fiend -> ready -= 1.0 * p -> talents.sin_and_punishment -> effect2().base_value();
      }
    }
  }

  virtual bool ready()
  {
    priest_t* p = player -> cast_priest();

    // Optional check to only cast Mind Flay if there's 2 or less ticks left on SW:P
    // This allows a action+=/mind_flay,swp_refresh to be added as a higher priority to ensure that SW:P doesn't fall off
    // Won't be necessary as part of a standard rotation, but if there's movement or other delays in casting it would have
    // it's uses.
    if ( swp_refresh && ( p -> talents.pain_and_suffering -> rank() > 0 ) )
    {
      if ( ! p -> dots_shadow_word_pain -> ticking )
        return false;

      if ( ( p -> dots_shadow_word_pain -> num_ticks -
             p -> dots_shadow_word_pain -> current_tick ) > 2 )
        return false;
    }

    // If this option is set (with a value in seconds), don't cast Mind Flay if Mind Blast
    // is about to come off it's cooldown.
    if ( mb_wait )
    {
      if ( p -> cooldowns_mind_blast -> remains() < mb_wait )
        return false;
    }

    return priest_spell_t::ready();
  }
};

// Mind Spike Spell =========================================================

struct mind_spike_t : public priest_spell_t
{
  mind_spike_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "mind_spike", player, "Mind Spike" )
  {
    parse_options( NULL, options_str );

  }

  virtual void execute()
  {
    priest_spell_t::execute();

    priest_t* p = player -> cast_priest();

    if ( p -> buffs_chakra_pre -> up() )
    {
      p -> buffs_chakra_chastise -> trigger();

      p -> buffs_chakra_pre -> expire();

      p -> cooldowns_chakra -> reset();
      p -> cooldowns_chakra -> duration = p -> buffs_chakra_pre -> spell_id_t::cooldown();
      p -> cooldowns_chakra -> duration -= p -> talents.state_of_mind -> effect1().seconds();
      p -> cooldowns_chakra -> start();
    }
  }

  virtual void travel( player_t* t, int travel_result, double dmg )
  {
    priest_t* p = player -> cast_priest();

    priest_spell_t::travel( t, travel_result, dmg );

    if ( result_is_hit( travel_result ) )
    {
      for ( int i=0; i < 4; i++ )
      {
        p -> uptimes_shadow_orb[ i ] -> update( i == p -> buffs_shadow_orb -> stack() );
      }

      p -> buffs_mind_melt  -> trigger( 1, 1.0 );
      p -> buffs_shadow_orb -> expire();
      p -> buffs_empowered_shadow -> trigger( 1, p -> empowered_shadows_amount() );
      p -> buffs_mind_spike -> trigger( 1, effect2().percent() );
    }
  }

  virtual void player_buff()
  {
    priest_t* p = player -> cast_priest();

    priest_spell_t::player_buff();

    player_multiplier *= 1.0 + p -> buffs_dark_archangel -> value() * p -> constants.dark_archangel_damage_value;

    player_multiplier *= 1.0 + ( p -> buffs_shadow_orb -> stack() * p -> shadow_orb_amount() );
  }
};

// Mind Spike Spell =========================================================

struct mind_sear_tick_t : public priest_spell_t
{
  mind_sear_tick_t( player_t* player ) :
    priest_spell_t( "mind_sear_tick", player, 49821 )
  {
    background  = true;
    dual        = true;
    direct_tick = true;

    stats = player -> get_stats( "mind_sear", this );
  }
};

struct mind_sear_t : public priest_spell_t
{
  mind_sear_tick_t* mind_sear_tick;

  mind_sear_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "mind_sear", player, "Mind Sear" ),
    mind_sear_tick( 0 )
  {
    parse_options( NULL, options_str );

    channeled = true;
    may_crit  = false;

    mind_sear_tick = new mind_sear_tick_t( player );
  }

  virtual void tick()
  {
    if ( mind_sear_tick )
      mind_sear_tick -> execute();

    stats -> add_tick( time_to_tick );
  }
};

// Penance Spell ============================================================

struct penance_tick_t : public priest_spell_t
{
  penance_tick_t( player_t* player ) :
    priest_spell_t( "penance_tick", player, 47666 )
  {
    background  = true;
    dual        = true;
    direct_tick = true;

    stats = player -> get_stats( "penance", this );
  }

  virtual void player_buff()
  {
    priest_spell_t::player_buff();

    priest_t* p = player -> cast_priest();

    player_multiplier *= 1.0 + ( p -> talents.evangelism -> rank() *
                                 p -> buffs_holy_evangelism -> stack() *
                                 p -> constants.holy_evangelism_damage_value );
  }
};

struct penance_t : public priest_spell_t
{
  spell_t* tick_spell;

  penance_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "penance", player, "Penance" ),
    tick_spell( 0 )
  {
    priest_t* p = player -> cast_priest();

    check_spec( TREE_DISCIPLINE );

    parse_options( NULL, options_str );

    harmful = false;
    channeled         = true;
    tick_zero         = true;

    num_ticks         = 2;
    base_tick_time    = 1.0;

    cooldown -> duration  += p -> glyphs.penance -> effect1().seconds();

    tick_spell = new penance_tick_t( p );

  }

  virtual void tick()
  {
    if ( sim -> debug ) log_t::output( sim, "%s ticks (%d of %d)", name(), dot -> current_tick, dot -> num_ticks );
    tick_spell -> execute();
    stats -> add_tick( time_to_tick );
  }

  virtual double cost() SC_CONST
  {
    double c = priest_spell_t::cost();

    priest_t* p = player -> cast_priest();

    c *= 1.0 - ( p -> talents.evangelism -> rank() *
                 p -> buffs_holy_evangelism -> check() *
                 p -> constants.holy_evangelism_mana_value );

    return c;
  }
};

// Power Infusion Spell =====================================================

struct power_infusion_t : public priest_spell_t
{
  player_t* power_infusion_target;

  power_infusion_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "power_infusion", player, "Power Infusion" )
  {
    priest_t* p = player -> cast_priest();

    check_talent( p -> talents.power_infusion -> rank() );

    std::string target_str = p -> power_infusion_target_str;

    option_t options[] =
    {
      { "target", OPT_STRING, &target_str },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    harmful = false;

    if ( target_str.empty() )
    {
      power_infusion_target = p;
    }
    else
    {
      power_infusion_target = sim -> find_player( target_str );
      assert ( power_infusion_target != 0 );
    }
  }

  virtual void execute()
  {
    priest_spell_t::execute();

    power_infusion_target -> buffs.power_infusion -> trigger();
  }

  virtual bool ready()
  {
    if ( power_infusion_target == 0 )
      return false;

    if ( power_infusion_target -> buffs.bloodlust -> check() )
      return false;

    if ( power_infusion_target -> buffs.power_infusion -> check() )
      return false;

    return priest_spell_t::ready();
  }
};

// Shadow Form Spell ========================================================

struct shadow_form_t : public priest_spell_t
{
  shadow_form_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "shadow_form", player, "Shadowform" )
  {
    priest_t* p = player -> cast_priest();
    parse_options( NULL, options_str );

    harmful = false;

    base_cost *= 1.0 + p -> talents.mental_agility -> mod_additive( P_RESOURCE_COST );
    base_cost  = floor( base_cost );
  }

  virtual double cost() SC_CONST
  {
    priest_t* p = player -> cast_priest();

    double c = priest_spell_t::cost();

    c *= 1.0 - p -> buffs_inner_will -> value();
    c  = floor( c );

    return c;
  }

  virtual void execute()
  {
    priest_t* p = player -> cast_priest();

    priest_spell_t::execute();

    p -> buffs_shadow_form -> trigger();

    sim -> auras.mind_quickening -> trigger();
  }

  virtual bool ready()
  {
    priest_t* p = player -> cast_priest();

    if (  p -> buffs_shadow_form -> check() )
      return false;

    return priest_spell_t::ready();
  }
};

// Shadow Word Death Spell ==================================================

struct shadow_word_death_t : public priest_spell_t
{
  double mb_min_wait;
  double mb_max_wait;

  shadow_word_death_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "shadow_word_death", player, "Shadow Word: Death" ), mb_min_wait( 0 ), mb_max_wait( 0 )
  {
    priest_t* p = player -> cast_priest();

    option_t options[] =
    {
      { "mb_min_wait",         OPT_FLT,  &mb_min_wait         },
      { "mb_max_wait",         OPT_FLT,  &mb_max_wait         },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    base_cost        *= 1.0 + p -> talents.mental_agility -> mod_additive( P_RESOURCE_COST );
    base_cost         = floor( base_cost );
  }

  virtual double cost() SC_CONST
  {
    priest_t* p = player -> cast_priest();

    double c = priest_spell_t::cost();

    c *= 1.0 - p -> buffs_inner_will -> value();
    c  = floor( c );

    return c;
  }

  virtual void execute()
  {
    priest_t* p = player -> cast_priest();

    p -> was_sub_25 = p -> glyphs.shadow_word_death -> ok() && ( ! p -> buffs_glyph_of_shadow_word_death -> check() ) &&
                      ( target -> health_percentage() <= 25 );

    priest_spell_t::execute();

    if ( result_is_hit() && p -> was_sub_25 && ( target -> health_percentage() > 0 ) )
    {
      cooldown -> reset();
      p -> buffs_glyph_of_shadow_word_death -> trigger();
    }
  }

  virtual void travel( player_t* t, int travel_result, double travel_dmg )
  {
    priest_t* p = player -> cast_priest();

    priest_spell_t::travel( t, travel_result, travel_dmg );

    double health_loss = travel_dmg * ( 1.0 - p -> talents.pain_and_suffering -> rank() * 0.20 );

    p -> resource_loss( RESOURCE_HEALTH, health_loss );

    if ( ( health_loss > 0.0 ) && p -> talents.masochism -> rank() )
    {
      p -> resource_gain( RESOURCE_MANA, 0.04 * p -> talents.masochism -> rank() * p -> resource_max[ RESOURCE_MANA ], p -> gains_masochism );
    }
  }

  virtual void player_buff()
  {
    priest_t* p = player -> cast_priest();

    priest_spell_t::player_buff();

    if ( p -> talents.mind_melt -> rank() && ( target -> health_percentage() <= 25 ) )
      player_multiplier *= 1.0 + p -> talents.mind_melt -> rank() * 0.15;

    if ( p -> glyphs.shadow_word_death -> ok() && ( target -> health_percentage() <= 25 ) )
      player_multiplier *= 3.0;

    player_multiplier *= 1.0 + p -> buffs_dark_archangel -> value() * p -> constants.dark_archangel_damage_value;
  }

  virtual bool ready()
  {
    priest_t* p = player -> cast_priest();

    if ( mb_min_wait )
      if ( p -> cooldowns_mind_blast -> remains() < mb_min_wait )
        return false;

    if ( mb_max_wait )
      if ( p -> cooldowns_mind_blast -> remains() > mb_max_wait )
        return false;

    return priest_spell_t::ready();
  }
};

// Shadow Word Pain Spell ===================================================

struct shadow_word_pain_t : public priest_spell_t
{
  shadow_word_pain_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "shadow_word_pain", player, "Shadow Word: Pain" )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    may_crit   = false;

    base_cost *= 1.0 + p -> talents.mental_agility -> mod_additive( P_RESOURCE_COST );
    base_cost  = floor( base_cost );

    stats -> children.push_back( player -> get_stats( "shadowy_apparition", this ) );
  }

  virtual double cost() SC_CONST
  {
    priest_t* p = player -> cast_priest();

    double c = priest_spell_t::cost();

    c *= 1.0 - p -> buffs_inner_will -> value();
    c  = floor( c );

    return c;
  }

  virtual void player_buff()
  {
    priest_t* p = player -> cast_priest();

    priest_spell_t::player_buff();

    player_td_multiplier += p -> constants.improved_shadow_word_pain_value;

    player_td_multiplier += p -> glyphs.shadow_word_pain -> ok() ? 0.1 : 0.0;
  }

  virtual void tick()
  {
    priest_spell_t::tick();

    priest_t* p = player -> cast_priest();

    // Shadowy Apparation
    trigger_shadowy_apparition( p );

    // Shadow Orb
    if ( result_is_hit() )
    {
      p -> buffs_shadow_orb  -> trigger( 1, 1, p -> constants.shadow_orb_proc_value + p -> constants.harnessed_shadows_value );
    }
  }

  virtual void refresh_duration()
  {
    num_ticks++;
    priest_spell_t::refresh_duration();
    num_ticks--;
  }
};

// Vampiric Embrace Spell ===================================================

struct vampiric_embrace_t : public priest_spell_t
{
  vampiric_embrace_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "vampiric_embrace", player, "Vampiric Embrace" )
  {
    priest_t* p = player -> cast_priest();

    parse_options( NULL, options_str );

    check_talent( p -> talents.vampiric_embrace -> rank() );

    harmful = false;
  }

  virtual void execute()
  {
    priest_t* p = player -> cast_priest();

    priest_spell_t::execute();

    if ( p -> talents.vampiric_embrace -> rank() )
      p -> buffs_vampiric_embrace -> trigger();
  }

  virtual bool ready()
  {
    priest_t* p = player -> cast_priest();

    if ( p -> buffs_vampiric_embrace -> check() )
      return false;

    return priest_spell_t::ready();
  }

};

// Vampiric Touch Spell =====================================================

struct vampiric_touch_t : public priest_spell_t
{
  vampiric_touch_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "vampiric_touch", player, "Vampiric Touch" )
  {
    parse_options( NULL, options_str );
    may_crit   = false;
  }

  virtual void execute()
  {
    priest_spell_t::execute();

    priest_t* p = player -> cast_priest();

    if ( result_is_hit() )
      p -> recast_mind_blast = 1;
  }
};

// Shadow Fiend Spell =======================================================

struct shadow_fiend_spell_t : public priest_spell_t
{
  shadow_fiend_spell_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "shadow_fiend", player, "Shadowfiend" )
  {
    parse_options( NULL, options_str );

    harmful = false;
  }

  virtual void execute()
  {
    priest_t* p = player -> cast_priest();

    priest_spell_t::execute();

    p -> summon_pet( "shadow_fiend", duration() );

  }
};

// Archangel Spell ==========================================================

struct archangel_t : public priest_spell_t
{
  archangel_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "archangel", player, "Archangel" )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    check_talent( p -> talents.archangel -> rank() );

    harmful = false;
  }

  virtual void execute()
  {
    priest_t* p = player -> cast_priest();

    double delta = p -> buffs_holy_evangelism -> last_trigger - p -> buffs_dark_evangelism -> last_trigger;

    if ( p -> buffs_holy_evangelism -> up() && delta > 0 )
    {
      cooldown -> duration = p -> dbc.effect( p -> dbc.spell( 87151 ) -> effect2().id() ) -> base_value();
      p -> buffs_holy_archangel -> trigger( 1, p -> buffs_holy_evangelism -> stack() );
      p -> resource_gain( RESOURCE_MANA, p -> resource_max[ RESOURCE_MANA ] * p -> constants.archangel_mana_value * p -> buffs_holy_evangelism -> stack(), p -> gains_archangel );
      p -> buffs_holy_evangelism -> expire();
    }

    else if ( p -> buffs_dark_evangelism -> up() && delta < 0 )
    {
      cooldown -> duration = p -> dbc.effect( p -> dbc.spell( 87151 ) -> effect3().id() ) -> base_value();
      p -> buffs_dark_archangel -> trigger( 1, p -> buffs_dark_evangelism -> stack() );
      p -> resource_gain( RESOURCE_MANA, p -> resource_max[ RESOURCE_MANA ] * p -> constants.dark_archangel_mana_value * p -> buffs_dark_evangelism -> stack(), p -> gains_archangel );
      p -> buffs_dark_evangelism -> expire();
    }

    priest_spell_t::execute();
  }

  virtual bool ready()
  {
    priest_t* p = player -> cast_priest();

    if ( ! ( p -> buffs_holy_evangelism -> check() || p -> buffs_dark_evangelism -> check() ) )
      return false;

    return priest_spell_t::ready();
  }
};

// Inner Focus Spell ========================================================

struct inner_focus_t : public priest_spell_t
{
  inner_focus_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "inner_focus", player, "Inner Focus" )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    harmful = false;

    check_talent( p -> talents.inner_focus -> rank() );
  }

  virtual void execute()
  {
    cooldown -> duration = sim -> wheel_seconds;

    priest_t* p = player -> cast_priest();

    priest_spell_t::execute();

    p -> buffs_inner_focus -> trigger();
  }
};

// Hymn of Hope Spell =======================================================

struct hymn_of_hope_tick_t : public priest_spell_t
{
  hymn_of_hope_tick_t( player_t* player ) :
    priest_spell_t( "hymn_of_hope_tick", player, 64904 )
  {
    dual        = true;
    background  = true;
    may_crit    = true;
    direct_tick = true;

    harmful     = false;

    stats = player -> get_stats( "hymn_of_hope", this );
  }

  virtual void execute()
  {
    priest_spell_t::execute();

    priest_t* p = player -> cast_priest();

    p -> resource_gain( RESOURCE_MANA, 0.02 * p -> resource_max[ RESOURCE_MANA ], p -> gains_hymn_of_hope );

    // Hymn of Hope only adds +x% of the current_max mana, it doesn't change if afterwards max_mana changes.
    p -> buffs.hymn_of_hope -> trigger();
  }

};

struct hymn_of_hope_t : public priest_spell_t
{
  hymn_of_hope_tick_t* hymn_of_hope_tick;

  hymn_of_hope_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "hymn_of_hope", player, "Hymn of Hope" ),
    hymn_of_hope_tick( 0 )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    harmful = false;

    channeled = true;

    hymn_of_hope_tick = new hymn_of_hope_tick_t( p );
  }

  virtual void tick()
  {
    hymn_of_hope_tick -> execute();

    stats -> add_tick( time_to_tick );
  }
};

// Chakra_Pre Spell =========================================================

struct chakra_t : public priest_spell_t
{
  chakra_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "chakra", player, "Chakra" )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    check_talent( p -> talents.chakra -> rank() );

    harmful = false;
  }

  virtual void execute()
  {
    cooldown -> duration = sim -> wheel_seconds;

    priest_spell_t::execute();

    priest_t* p = player -> cast_priest();

    p -> buffs_chakra_pre -> trigger();
  }

  virtual bool ready()
  {
    priest_t* p = player -> cast_priest();

    if ( p -> buffs_chakra_pre -> up() )
      return false;

    return priest_spell_t::ready();
  }
};

// Smite non-crit Atonement Heal ============================================

struct atonement_nc_t : public priest_heal_t
{
  atonement_nc_t( player_t* player ) :
    priest_heal_t( "atonement", player, 81751 )
  {
    proc       = true;
    background = true;
  }

  virtual void calculate_result()
  {
    result = RESULT_HIT;
  }

  virtual void target_debuff( player_t* t, int dmg_type )
  {
    priest_heal_t::target_debuff( t, dmg_type );

    priest_t* p = player -> cast_priest();

    if ( p -> bugs && t -> buffs.grace -> up() )
      target_multiplier /= 1.0 + t -> buffs.grace -> value();
  }
};

// Smite non-crit Atonement Heal ============================================

struct atonement_c_t : public priest_heal_t
{
  // Missusing ID 81751 for now, correct id is: 94472
  atonement_c_t( player_t* player ) :
    priest_heal_t( "atonement", player, 81751 )
  {
    proc       = true;
    background = true;
  }

  virtual void calculate_result()
  {
    result = RESULT_CRIT;
  }

  virtual double total_crit_bonus() SC_CONST
  {
    return 0;
  }

  virtual void target_debuff( player_t* t, int dmg_type )
  {
    priest_heal_t::target_debuff( t, dmg_type );

    priest_t* p = player -> cast_priest();

    if ( p -> bugs && t -> buffs.grace -> up() )
      target_multiplier /= 1.0 + t -> buffs.grace -> value();
  }
};

// Smite Spell ==============================================================

struct smite_t : public priest_spell_t
{
  smite_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "smite", player, "Smite" )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    base_execute_time += p -> talents.divine_fury -> effect1().seconds();

    base_hit       += p -> glyphs.divine_accuracy -> ok() ? 0.18 : 0.0;

  }

  virtual void execute()
  {
    priest_t* p = player -> cast_priest();

    priest_spell_t::execute();

    if ( p -> buffs_surge_of_light -> trigger() )
      p -> procs_surge_of_light -> occur();

    p -> buffs_holy_evangelism  -> trigger();

    // Chakra
    if ( p -> buffs_chakra_pre -> up() )
    {
      p -> buffs_chakra_chastise -> trigger();

      p -> buffs_chakra_pre -> expire();

      p -> cooldowns_chakra -> reset();
      p -> cooldowns_chakra -> duration = p -> buffs_chakra_pre -> spell_id_t::cooldown();
      p -> cooldowns_chakra -> duration -= p -> talents.state_of_mind -> effect1().seconds();
      p -> cooldowns_chakra -> start();
    }

    trigger_atonement( player, result, direct_dmg );

    // Train of Thought
    if ( p -> talents.train_of_thought -> rank() )
    {
      if ( p -> cooldowns_penance -> remains() > p -> talents.train_of_thought -> effect2().seconds() )
        p -> cooldowns_penance -> ready -= p -> talents.train_of_thought -> effect2().seconds();
      else
        p -> cooldowns_penance -> reset();
    }
  }

  virtual void player_buff()
  {
    priest_spell_t::player_buff();

    priest_t* p = player -> cast_priest();

    player_multiplier *= 1.0 + ( p -> talents.evangelism -> rank() * p -> buffs_holy_evangelism -> stack() * p -> constants.holy_evangelism_damage_value );

    if ( p -> dots_holy_fire -> ticking && p -> glyphs.smite -> ok() )
      player_multiplier *= 1.0 + p -> glyphs.smite -> effect1().percent();
  }

  virtual double cost() SC_CONST
  {
    double c = priest_spell_t::cost();

    priest_t* p = player -> cast_priest();

    c *= 1.0 - ( p -> talents.evangelism -> rank() *
    p -> buffs_holy_evangelism -> check() *
    p -> constants.holy_evangelism_mana_value );

    return c;
  }
};

// Renew Spell ==============================================================

struct divine_touch_t : public priest_heal_t
{
  divine_touch_t( player_t* player ) :
    priest_heal_t( "divine_touch", player, 63544 )
  {
    school          = SCHOOL_HOLY;
    stats -> school = school;

    background = true;
    proc       = true;
  }
};

struct renew_t : public priest_heal_t
{
  divine_touch_t* dt;

  renew_t( player_t* player, const std::string& options_str ) :
    priest_heal_t( "renew", player, "Renew" ), dt( 0 )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    base_cost *= 1.0 + p -> talents.mental_agility -> mod_additive( P_RESOURCE_COST );
    base_cost  = floor( base_cost );

    base_multiplier *= 1.0 + p -> glyphs.renew -> effect1().percent();
    base_multiplier *= 1.0 + p -> talents.improved_renew -> effect1().percent();

    // Implement Divine Touch
    if ( p -> talents.divine_touch -> rank() )
    {
      dt = new divine_touch_t( p );

      add_child( dt );
    }

    if ( p -> talents.rapid_renewal -> rank() )
      trigger_gcd += p -> talents.rapid_renewal -> effect1().seconds();
  }

  virtual void travel( player_t* t, int travel_result, double travel_dmg )
  {
    priest_heal_t::travel( t, travel_result, travel_dmg );

    // Divine Touch
    if ( dt )
    {
      priest_t* p = player -> cast_priest();
      dt -> base_dd_min = dt -> base_dd_max = ( base_td + total_power() * tick_power_mod ) * hasted_num_ticks() * p -> talents.divine_touch -> effect1().percent();
      dt -> heal_target.clear();
      dt -> heal_target.push_back( t );
      dt -> execute();
    }
  }

  virtual double cost() SC_CONST
  {
    double c = priest_heal_t::cost();

    priest_t* p = player -> cast_priest();

    c *= 1.0 - p -> buffs_inner_will -> value();
    c  = floor( c );

    return c;
  }

  virtual void player_buff()
  {
    priest_heal_t::player_buff();

    priest_t* p = player -> cast_priest();

    if ( p -> buffs_chakra_sanctuary -> up() )
      player_multiplier *= 1.0 + p -> buffs_chakra_sanctuary -> effect1().percent();
  }
};

// Heal Spell ===============================================================

struct _heal_t : public priest_heal_t
{
  _heal_t( player_t* player, const std::string& options_str ) :
    priest_heal_t( "heal", player, "Heal" )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    base_execute_time += p -> talents.divine_fury -> effect1().seconds();
    base_multiplier *= 1.0 + p -> talents.empowered_healing -> base_value( E_APPLY_AURA , A_ADD_PCT_MODIFIER );

    // FIXME: Check Spell when it's imported into the dbc
    base_crit      += p -> set_bonus.tier11_2pc_heal() * 0.05;
  }

  virtual void execute()
  {
    priest_heal_t::execute();
    priest_t* p = player -> cast_priest();

    if ( p -> buffs_surge_of_light -> trigger() )
      p -> procs_surge_of_light -> occur();

    p -> buffs_divine_fire -> trigger();

    // Trigger Chakra
    if ( p -> buffs_chakra_pre -> up() )
    {
      p -> buffs_chakra_serenity -> trigger();

      p -> buffs_chakra_pre -> expire();

      p -> cooldowns_chakra -> reset();
      p -> cooldowns_chakra -> duration = p -> buffs_chakra_pre -> spell_id_t::cooldown();
      p -> cooldowns_chakra -> duration -= p -> talents.state_of_mind -> effect1().seconds();
      p -> cooldowns_chakra -> start();
    }
  }

  virtual void travel( player_t* t, int travel_result, double travel_dmg )
  {
    priest_heal_t::travel( t, travel_result, travel_dmg );

    trigger_inspiration( travel_result, t );

    trigger_grace( t );
    trigger_strength_of_soul( t );
  }

  virtual void target_debuff( player_t* t, int dmg_type )
  {
    priest_heal_t::target_debuff( t, dmg_type );

    priest_t* p = player -> cast_priest();

    if ( t -> buffs.grace -> up() || p -> buffs_weakened_soul -> up() )
      player_crit += p -> talents.renewed_hope -> effect1().percent();
  }
};

// Flash Heal Spell =========================================================

struct flash_heal_t : public priest_heal_t
{
  flash_heal_t( player_t* player, const std::string& options_str ) :
    priest_heal_t( "flash_heal", player, "Flash Heal" )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    base_multiplier *= 1.0 + p -> talents.empowered_healing -> base_value( E_APPLY_AURA , A_ADD_PCT_MODIFIER );
  }

  virtual void execute()
  {
    priest_t* p = player -> cast_priest();

    priest_heal_t::execute();

    // Assuming a SoL Flash Heal can't proc SoL
    if ( p -> buffs_surge_of_light -> up() )
      p -> buffs_surge_of_light -> expire();
    else if ( p -> buffs_surge_of_light -> trigger() )
      p -> procs_surge_of_light -> occur();

    p -> buffs_divine_fire -> trigger();

    p -> buffs_serendipity -> trigger();

    // Inner Focus
    if ( p -> buffs_inner_focus -> up() )
    {
      // Inner Focus cooldown starts when consumed.
      p -> cooldowns_inner_focus -> reset();
      p -> cooldowns_inner_focus -> duration = p -> buffs_inner_focus -> spell_id_t::cooldown();
      p -> cooldowns_inner_focus -> start();
      p -> buffs_inner_focus -> expire();
    }

    // Chakra
    if ( p -> buffs_chakra_pre -> up() )
    {
      p -> buffs_chakra_serenity -> trigger();

      p -> buffs_chakra_pre -> expire();

      p -> cooldowns_chakra -> reset();
      p -> cooldowns_chakra -> duration = p -> buffs_chakra_pre -> spell_id_t::cooldown();
      p -> cooldowns_chakra -> duration -= p -> talents.state_of_mind -> effect1().seconds();
      p -> cooldowns_chakra -> start();
    }
  }

  virtual void travel( player_t* t, int travel_result, double travel_dmg )
  {
    priest_heal_t::travel( t, travel_result, travel_dmg );

    trigger_inspiration( travel_result, t );

    trigger_grace( t );
    trigger_strength_of_soul( t );
  }

  virtual void player_buff()
  {
    priest_heal_t::player_buff();

    priest_t* p = player -> cast_priest();

    if ( p -> buffs_inner_focus -> up() )
      player_crit += p -> buffs_inner_focus -> effect2().percent();
  }

  virtual void target_debuff( player_t* t, int dmg_type )
  {
    priest_heal_t::target_debuff( t, dmg_type );

    priest_t* p = player -> cast_priest();

    if ( t -> buffs.grace -> up() || p -> buffs_weakened_soul -> up() )
      player_crit += p -> talents.renewed_hope -> effect1().percent();
  }

  virtual double cost() SC_CONST
  {
    double c = priest_heal_t::cost();

    priest_t* p = player -> cast_priest();

    if ( p -> buffs_inner_focus -> check() )
      c = 0;

    if ( p -> buffs_surge_of_light -> check() )
      c = 0;

    return c;
  }

  virtual double execute_time() SC_CONST
  {
    priest_t* p = player -> cast_priest();

    if ( p -> buffs_surge_of_light -> check() )
      return 0;

    return priest_heal_t::execute_time();
  }
};

// Binding Heal Spell =======================================================

struct binding_heal_t : public priest_heal_t
{
  binding_heal_t( player_t* player, const std::string& options_str ) :
    priest_heal_t( "binding_heal", player, "Binding Heal" )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    base_multiplier *= 1.0 + p -> talents.empowered_healing -> base_value( E_APPLY_AURA , A_ADD_PCT_MODIFIER );
  }

  virtual void execute()
  {
    // Adjust heal_target
    priest_t* p = player -> cast_priest();
    if ( heal_target[0] != p )
      heal_target.push_back( p );
    else
    {
      for ( player_t* q = sim -> player_list; q; q = q -> next )
      {
        if ( !q -> is_pet() && q != heal_target[0] && p -> get_player_distance( q ) < ( range * range ) )
        {
          heal_target.push_back( q );
          break;
        }
      }
    }

    priest_heal_t::execute();

    p -> buffs_serendipity -> trigger( 1 );

    if ( p -> buffs_surge_of_light -> trigger() )
      p -> procs_surge_of_light -> occur();

    // Inner Focus
    if ( p -> buffs_inner_focus -> up() )
    {
      // Inner Focus cooldown starts when consumed.
      p -> cooldowns_inner_focus -> reset();
      p -> cooldowns_inner_focus -> duration = p -> buffs_inner_focus -> spell_id_t::cooldown();
      p -> cooldowns_inner_focus -> start();
      p -> buffs_inner_focus -> expire();
    }

    // Chakra
    if ( p -> buffs_chakra_pre -> up() )
    {
      p -> buffs_chakra_serenity -> trigger();

      p -> buffs_chakra_pre -> expire();

      p -> cooldowns_chakra -> reset();
      p -> cooldowns_chakra -> duration = p -> buffs_chakra_pre -> spell_id_t::cooldown();
      p -> cooldowns_chakra -> duration -= p -> talents.state_of_mind -> effect1().seconds();
      p -> cooldowns_chakra -> start();
    }
  }

  virtual void travel( player_t* t, int travel_result, double travel_dmg )
  {
    priest_heal_t::travel( t, travel_result, travel_dmg );

    trigger_inspiration( travel_result, t );
  }

  virtual void player_buff()
  {
    priest_heal_t::player_buff();

    priest_t* p = player -> cast_priest();

    if ( p -> buffs_inner_focus -> up() )
      player_crit += p -> buffs_inner_focus -> effect2().percent();
  }

  virtual double cost() SC_CONST
  {
    priest_t* p = player -> cast_priest();

    double c = priest_heal_t::cost();

    if ( p -> buffs_inner_focus -> check() )
      c = 0;

    return c;
  }
};

// Greater Heal Spell =======================================================

struct greater_heal_t : public priest_heal_t
{
  greater_heal_t( player_t* player, const std::string& options_str ) :
    priest_heal_t( "greater_heal", player, "Greater Heal" )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    base_execute_time += p -> talents.divine_fury -> effect1().seconds();

    base_multiplier *= 1.0 + p -> talents.empowered_healing -> base_value( E_APPLY_AURA , A_ADD_PCT_MODIFIER );
  }

  virtual void execute()
  {
    priest_heal_t::execute();

    priest_t* p = player -> cast_priest();

    p -> buffs_serendipity -> expire();

    p -> buffs_divine_fire -> trigger();

    if ( p -> buffs_surge_of_light -> trigger() )
      p -> procs_surge_of_light -> occur();

    // Inner Focus
    if ( p -> buffs_inner_focus -> up() )
    {
      // Inner Focus cooldown starts when consumed.
      p -> cooldowns_inner_focus -> reset();
      p -> cooldowns_inner_focus -> duration = p -> buffs_inner_focus -> spell_id_t::cooldown();
      p -> cooldowns_inner_focus -> start();
      p -> buffs_inner_focus -> expire();
    }

    // Chakra
    if ( p -> buffs_chakra_pre -> up() )
    {
      p -> buffs_chakra_serenity -> trigger();

      p -> buffs_chakra_pre -> expire();

      p -> cooldowns_chakra -> reset();
      p -> cooldowns_chakra -> duration = p -> buffs_chakra_pre -> spell_id_t::cooldown();
      p -> cooldowns_chakra -> duration -= p -> talents.state_of_mind -> effect1().seconds();
      p -> cooldowns_chakra -> start();
    }

    // Train of Thought
    if ( p -> talents.train_of_thought -> rank() )
    {
      if ( p -> cooldowns_inner_focus -> remains() > p -> talents.train_of_thought -> effect1().base_value() )
        p -> cooldowns_inner_focus -> ready -= p -> talents.train_of_thought -> effect1().base_value();
      else
        p -> cooldowns_inner_focus -> reset();
    }
  }

  virtual void travel( player_t* t, int travel_result, double travel_dmg )
  {
    priest_heal_t::travel( t, travel_result, travel_dmg );

    trigger_inspiration( travel_result, t );

    trigger_grace( t );
    trigger_strength_of_soul( t );
  }

  virtual void player_buff()
  {
    priest_heal_t::player_buff();

    priest_t* p = player -> cast_priest();

    if ( p -> buffs_inner_focus -> up() )
      player_crit += p -> buffs_inner_focus -> effect2().percent();
  }

  virtual void target_debuff( player_t* t, int dmg_type )
  {
    priest_heal_t::target_debuff( t, dmg_type );

    priest_t* p = player -> cast_priest();

    if ( t -> buffs.grace -> up() || p -> buffs_weakened_soul -> up() )
      player_crit += p -> talents.renewed_hope -> effect1().percent();
  }

  virtual double execute_time() SC_CONST
  {
    priest_t* p = player -> cast_priest();

    double c = priest_heal_t::execute_time();

    if ( p -> buffs_serendipity -> check() )
      c *= 1.0 + p -> buffs_serendipity -> effect1().percent();

    return c;
  }

  virtual double cost() SC_CONST
  {
    priest_t* p = player -> cast_priest();

    double c = priest_heal_t::cost();

    if ( p -> buffs_serendipity -> check() )
      c *= 1.0 + p -> buffs_serendipity -> effect2().percent();

    if ( p -> buffs_inner_focus -> check() )
      c = 0;

    return c;
  }
};

// Prayer of Healing Spell ==================================================

struct glyph_prayer_of_healing_t : public priest_heal_t
{
  glyph_prayer_of_healing_t( player_t* player ) :
    priest_heal_t( "prayer_of_healing_glyph", player, 56161 )
  {
    proc       = true;
    background = true;
    may_crit   = false;

    num_ticks=0; // coded as DD for now.
  }
};

struct prayer_of_healing_t : public priest_heal_t
{
  glyph_prayer_of_healing_t* glyph;

  prayer_of_healing_t( player_t* player, const std::string& options_str ) :
    priest_heal_t( "prayer_of_healing", player, "Prayer of Healing" ), glyph( 0 )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    if ( p -> glyphs.prayer_of_healing -> ok() )
    {
      glyph = new glyph_prayer_of_healing_t( p );
      add_child( glyph );
    }
  }

  virtual void execute()
  {
    // Choose Heal Targets
    priest_t* p = player -> cast_priest();
    unsigned int h = 0;
    heal_target.resize( 1 ); // Only leave the first target, reset the others.
    for ( player_t* q = sim -> player_list; q; q = q -> next )
    {
      if( h > 3 ) continue; // This is actually not correct, but since simc partys aren't automatically limited to 5 people it would screw the results on large simulations.
      if ( !q -> is_pet() && q != heal_target[0] && q -> get_player_distance( heal_target[0] ) < ( range * range ) && q -> party == p -> party )
      {
        heal_target.push_back( q );
      }
      h++;
    }

    priest_heal_t::execute();

    p -> buffs_serendipity -> expire();

    // Inner Focus
    if ( p -> buffs_inner_focus -> up() )
    {
      // Inner Focus cooldown starts when consumed.
      p -> cooldowns_inner_focus -> reset();
      p -> cooldowns_inner_focus -> duration = p -> buffs_inner_focus -> spell_id_t::cooldown();
      p -> cooldowns_inner_focus -> start();
      p -> buffs_inner_focus -> expire();
    }

    // Chakra
    if ( p -> buffs_chakra_pre -> up() )
    {
      p -> buffs_chakra_sanctuary -> trigger();

      p -> buffs_chakra_pre -> expire();

      p -> cooldowns_chakra -> reset();
      p -> cooldowns_chakra -> duration = p -> buffs_chakra_pre -> spell_id_t::cooldown();
      p -> cooldowns_chakra -> duration -= p -> talents.state_of_mind -> effect1().seconds();
      p -> cooldowns_chakra -> start();
    }
  }

  virtual void travel( player_t* t, int travel_result, double travel_dmg )
  {
    priest_heal_t::travel( t, travel_result, travel_dmg );

    trigger_inspiration( travel_result, t );

    // Glyph
    if ( glyph )
    {
      priest_t* p = player -> cast_priest();

      // FIXME: Modelled as a Direct Heal instead of a dot
      glyph -> heal_target.clear(); glyph -> heal_target.push_back( t );
      glyph -> base_dd_min = glyph -> base_dd_max = travel_dmg * p -> glyphs.prayer_of_healing -> effect1().percent();
      glyph -> execute();
    }

    // Divine Aegis
    if ( da && travel_result != RESULT_CRIT )
    {
      da -> base_dd_min = travel_dmg * 0.3;
      da -> base_dd_max = travel_dmg * 0.3;
      da -> heal_target.clear();
      da -> heal_target.push_back( t );
      da -> execute();
    }
  }

  virtual void player_buff()
  {
    priest_heal_t::player_buff();

    priest_t* p = player -> cast_priest();

    if ( p -> buffs_inner_focus -> up() )
      player_crit += p -> buffs_inner_focus -> effect2().percent();

    if ( p -> buffs_chakra_sanctuary -> up() )
      player_multiplier *= 1.0 + p -> buffs_chakra_sanctuary -> effect1().percent();
  }

  virtual double execute_time() SC_CONST
  {
    priest_t* p = player -> cast_priest();

    double c = priest_heal_t::execute_time();

    if ( p -> buffs_serendipity -> check() )
      c *= 1.0 + p -> buffs_serendipity -> effect1().percent();

    return c;
  }

  virtual double cost() SC_CONST
  {
    priest_t* p = player -> cast_priest();

    double c = priest_heal_t::cost();

    if ( p -> buffs_serendipity -> check() )
      c *= 1.0 + p -> buffs_serendipity -> effect2().percent();

    if ( p -> buffs_inner_focus -> check() )
      c = 0;

    return c;
  }
};

// Circle of Healing ========================================================

struct circle_of_healing_t : public priest_heal_t
{
  circle_of_healing_t( player_t* player, const std::string& options_str ) :
    priest_heal_t( "circle_of_healing", player, "Circle of Healing" )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    check_talent( p -> talents.circle_of_healing -> rank() );

    base_cost *= 1.0 + p -> talents.mental_agility -> mod_additive( P_RESOURCE_COST );
    base_cost         = floor( base_cost );
  }

  virtual double cost() SC_CONST
  {
    priest_t* p = player -> cast_priest();

    double c = priest_heal_t::cost();

    c *= 1.0 - p -> buffs_inner_will -> value();
    c  = floor( c );

    return c;
  }

  virtual void execute()
  {
    priest_t* p = player -> cast_priest();

    cooldown -> duration = spell_id_t::cooldown();

    if ( p -> buffs_chakra_sanctuary -> up() )
      cooldown -> duration +=  p -> buffs_chakra_sanctuary -> effect2().seconds();

    // Choose Heal Target
    int h = 0;
    heal_target.clear();
    heal_target.push_back( find_lowest_player() );
    for ( player_t* q = sim -> player_list; q; q = q -> next )
    {
      if( h > ( p -> glyphs.circle_of_healing -> ok() ? 4 : 3 ) ) continue;
      if ( !q -> is_pet() && q != player && q -> get_player_distance( heal_target[0] ) < ( range * range ) )
      {
        heal_target.push_back( q );
      }
      h++;
    }

    priest_heal_t::execute();
  }

  virtual void travel( player_t* t, int travel_result, double travel_dmg )
  {
    priest_heal_t::travel( t, travel_result, travel_dmg );

    trigger_inspiration( travel_result, t );
  }

  virtual void player_buff()
  {
    priest_heal_t::player_buff();

    priest_t* p = player -> cast_priest();

    if ( p -> buffs_chakra_sanctuary -> up() )
      player_multiplier *= 1.0 + p -> buffs_chakra_sanctuary -> effect1().percent();
  }
};

// Prayer of Mending Spell ==================================================

struct prayer_of_mending_t : public priest_heal_t
{
  int single;
  prayer_of_mending_t( player_t* player, const std::string& options_str ) :
    priest_heal_t( "prayer_of_mending", player, "Prayer of Mending" ), single( false )
  {
    option_t options[] =
    {
      { "single", OPT_BOOL,       &single },
      { NULL,     OPT_UNKNOWN, NULL       }
    };
    parse_options( options, options_str );

    priest_t* p = player -> cast_priest();

    direct_power_mod = effect_coeff( 1 );
    base_dd_min = base_dd_max = effect_min( 1 );

    base_cost *= 1.0 + p -> talents.mental_agility -> mod_additive( P_RESOURCE_COST );
    base_cost  = floor( base_cost );
  }

  virtual void player_buff()
  {
    priest_heal_t::player_buff();

    priest_t* p = player -> cast_priest();

    if ( p -> buffs_chakra_sanctuary -> up() )
      player_multiplier *= 1.0 + p -> buffs_chakra_sanctuary -> effect1().percent();
  }

  virtual double cost() SC_CONST
  {
    priest_t* p = player -> cast_priest();

    double c = priest_heal_t::cost();

    c *= 1.0 - p -> buffs_inner_will -> value();
    c  = floor( c );

    return c;
  }

  virtual void execute()
  {
    // Set Heal Targets
    heal_target.resize( 1 ); // Only leave the first target, reset the others.
    if ( ! single )
    {
      unsigned int h = 0;
      for ( player_t* p = sim -> player_list; p; p = p -> next )
      {
        if( h > 3 ) continue;
        if ( !p -> is_pet() && p != player )
        {
          heal_target.push_back( p );
        }
        h++;
      }
    }

    priest_heal_t::execute();

    priest_t* p = player -> cast_priest();

    p -> buffs_divine_fire -> trigger();

    // Chakra
    if ( p -> buffs_chakra_pre -> up() )
    {
      p -> buffs_chakra_sanctuary -> trigger();

      p -> buffs_chakra_pre -> expire();

      p -> cooldowns_chakra -> reset();
      p -> cooldowns_chakra -> duration = p -> buffs_chakra_pre -> spell_id_t::cooldown();
      p -> cooldowns_chakra -> duration -= p -> talents.state_of_mind -> effect1().seconds();
      p -> cooldowns_chakra -> start();
    }
  }

  virtual void target_debuff( player_t* t, int dmg_type )
  {
    priest_heal_t::target_debuff( t, dmg_type );

    priest_t* p = player -> cast_priest();

    if ( p -> glyphs.prayer_of_mending -> ok() && t == heal_target[0] )
      target_multiplier *= 1.0 + p -> glyphs.prayer_of_mending -> effect1().percent();
  }
};

// Power Word: Shield Spell =================================================

struct glyph_power_word_shield_t : public priest_heal_t
{
  glyph_power_word_shield_t( player_t* player ) :
    priest_heal_t( "power_word_shield_glyph", player, 55672 )
  {
    school          = SCHOOL_HOLY;
    stats -> school = school;

    background = true;
    proc       = true;
  }
};

// Weakened Soul doesn't block PW:S yet to enable shield spam simulations
struct power_word_shield_t : public priest_absorb_t
{
  glyph_power_word_shield_t* glyph_pws;

  power_word_shield_t( player_t* player, const std::string& options_str ) :
    priest_absorb_t( "power_word_shield", player, 17 ), glyph_pws( 0 )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    cooldown -> duration += p -> talents.soul_warding -> effect1().seconds();

    base_cost *= 1.0 + p -> talents.mental_agility -> mod_additive( P_RESOURCE_COST );
    base_cost  = floor( base_cost );

    base_multiplier *= 1.0 + p -> talents.improved_power_word_shield -> effect1().percent();

    direct_power_mod = 0.87; // hardcoded into tooltip

    if ( p -> glyphs.power_word_shield -> ok() )
    {
      glyph_pws = new glyph_power_word_shield_t( p );
      add_child( glyph_pws );
    }
  }

  virtual double cost() SC_CONST
  {
    priest_t* p = player -> cast_priest();

    double c = priest_absorb_t::cost();

    c *= 1.0 - p -> buffs_inner_will -> value();
    c  = floor( c );

    return c;
  }

  virtual void execute()
  {
    priest_absorb_t::execute();

    priest_t* p = player -> cast_priest();

    p -> buffs_borrowed_time -> trigger();
    p -> buffs_weakened_soul -> trigger();

    // Rapture
    if ( p -> cooldowns_rapture -> remains() == 0 && p -> talents.rapture -> rank() )
    {
      // FIX-ME: Hotfix on Feb 18th, 2011: http://blue.mmo-champion.com/topic/157148/patch-406-hotfixes-february-18
      double amount = util_t::talent_rank( p -> talents.rapture -> rank(), 3, 0.02, 0.05, 0.07 );
      p -> resource_gain( RESOURCE_MANA, p -> resource_max[ RESOURCE_MANA ] * amount, p -> gains_rapture );
      p -> cooldowns_rapture -> start();
    }
  }

  virtual void travel( player_t* t, int travel_result, double travel_dmg )
  {
    priest_absorb_t::travel( t, travel_result, travel_dmg );

    priest_t* p = player -> cast_priest();

    // Glyph
    if ( glyph_pws )
    {

      glyph_pws -> base_dd_min  = glyph_pws -> base_dd_max  = p -> glyphs.power_word_shield -> effect1().percent() * travel_dmg;
      glyph_pws -> heal_target.clear();
      glyph_pws -> heal_target.push_back( t );
      glyph_pws -> execute();
    }

    // Body and Soul
    if ( p -> talents.body_and_soul -> rank() )
      t -> buffs.body_and_soul -> trigger( 1, p -> talents.body_and_soul -> effect1().percent() );
  }
};

// Penance Heal Spell =======================================================

struct penance_heal_tick_t : public priest_heal_t
{
  penance_heal_tick_t( player_t* player ) :
    priest_heal_t( "penance_heal_tick", player, 47750 )
  {
    background  = true;
    may_crit    = true;
    dual        = true;
    direct_tick = true;

    stats = player -> get_stats( "penance_heal", this );
  }

  virtual void execute()
  {
    priest_heal_t::execute();

    priest_t* p = player -> cast_priest();

    p -> buffs_indulgence_of_the_penitent -> trigger();
  }

  virtual void travel( player_t* t, int travel_result, double travel_dmg )
  {
    priest_heal_t::travel( t, travel_result, travel_dmg );

    trigger_inspiration( travel_result, t );
    trigger_grace( t );
  }

  virtual void target_debuff( player_t* t, int dmg_type )
  {
    priest_heal_t::target_debuff( t, dmg_type );

    priest_t* p = player -> cast_priest();

    if ( t -> buffs.grace -> up() || p -> buffs_weakened_soul -> up() )
      player_crit += p -> talents.renewed_hope -> effect1().percent();
  }
};

struct penance_heal_t : public priest_heal_t
{
  penance_heal_tick_t* penance_tick;

  penance_heal_t( player_t* player, const std::string& options_str ) :
    priest_heal_t( "penance_heal", player, 47540 ), penance_tick( 0 )
  {
    priest_t* p = player -> cast_priest();

    check_spec( TREE_DISCIPLINE );

    parse_options( NULL, options_str );

    may_crit = false;

    channeled         = true;
    tick_zero         = true;

    num_ticks         = 2;
    base_tick_time    = 1.0;

    cooldown = player -> get_cooldown( "penance" );
    cooldown -> duration = spell_id_t::cooldown();
    cooldown -> duration  += p -> glyphs.penance -> effect1().seconds();

    penance_tick = new penance_heal_tick_t( p );
  }

  virtual void tick()
  {
    if ( sim -> debug ) log_t::output( sim, "%s ticks (%d of %d)", name(), dot -> current_tick, dot -> num_ticks );
    penance_tick -> execute();
    stats -> add_tick( time_to_tick );
  }

  virtual double cost() SC_CONST
  {
    double c = priest_heal_t::cost();

    priest_t* p = player -> cast_priest();

    c *= 1.0 - ( p -> talents.evangelism -> rank() *
    p -> buffs_holy_evangelism -> check() *
    p -> constants.holy_evangelism_mana_value );

    return c;
  }
};

// Holy Word Spell ==========================================================

struct holy_word_sanctuary_tick_t : public priest_heal_t
{
  holy_word_sanctuary_tick_t( player_t* player ) :
    priest_heal_t( "holy_word_sanctuary_tick", player, 88686 )
  {
    dual        = true;
    background  = true;
    direct_tick = true;

    stats = player -> get_stats( "holy_word_sanctuary", this );
  }
  virtual void execute()
  {
    // Choose Heal Targets
    heal_target.clear();
    for ( player_t* q = sim -> player_list; q; q = q -> next )
    {
      if ( !q -> is_pet() && q -> get_player_distance( player ) < ( range * range ) )
      {
        heal_target.push_back( q );
      }
    }

    priest_heal_t::execute();
  }

  virtual void player_buff()
  {
    priest_heal_t::player_buff();

    priest_t* p = player -> cast_priest();

    if ( p -> buffs_chakra_sanctuary -> up() )
      player_multiplier *= 1.0 + p -> buffs_chakra_sanctuary -> effect1().percent();
  }
};

struct holy_word_sanctuary_t : public priest_heal_t
{
  spell_t* tick_spell;

  holy_word_sanctuary_t( player_t* player ) :
    priest_heal_t( "holy_word_sanctuary", player, 88685 ),
    tick_spell( 0 )
  {
    hasted_ticks = false;
    may_crit     = false;

    base_tick_time = 2.0;
    num_ticks = 9;

    priest_t* p = player -> cast_priest();
    tick_spell = new holy_word_sanctuary_tick_t( p );


    cooldown -> duration *= 1.0 + p -> talents.tome_of_light -> effect1().percent();
  }

  virtual void tick()
  {
    tick_spell -> execute();
    stats -> add_tick( time_to_tick );
  }

  virtual bool ready()
  {
    priest_t* p = player -> cast_priest();

    if ( ! p -> talents.revelations -> rank() )
      return false;

    if ( ! p -> buffs_chakra_sanctuary -> check() )
      return false;

    return priest_heal_t::ready();
  }
};

struct holy_word_chastise_t : public priest_spell_t
{
  holy_word_chastise_t( player_t* player ) :
    priest_spell_t( "holy_word_chastise", player, 88625 )
  {
    priest_t* p = player -> cast_priest();

    base_cost *= 1.0 + p -> talents.mental_agility -> mod_additive( P_RESOURCE_COST );
    base_cost  = floor( base_cost );

    cooldown -> duration *= 1.0 + p -> talents.tome_of_light -> effect1().percent();
  }

  virtual bool ready()
  {
    priest_t* p = player -> cast_priest();

    if ( p -> buffs_chakra_sanctuary -> check() )
      return false;

    if (  p -> buffs_chakra_serenity -> check() )
      return false;

    return priest_spell_t::ready();
  }
};

struct holy_word_serenity_t : public priest_heal_t
{
  holy_word_serenity_t( player_t* player ) :
    priest_heal_t( "holy_word_serenity", player, 88684 )
  {
    priest_t* p = player -> cast_priest();

    base_cost *= 1.0 + p -> talents.mental_agility -> mod_additive( P_RESOURCE_COST );
    base_cost  = floor( base_cost );

    cooldown -> duration *= 1.0 + p -> talents.tome_of_light -> effect1().percent();
  }

  virtual void execute()
  {
    priest_heal_t::execute();

    priest_t* p = player -> cast_priest();

    p -> buffs_serenity -> trigger();
  }

  virtual bool ready()
  {
    priest_t* p = player -> cast_priest();

    if ( ! p -> talents.revelations -> rank() )
      return false;

    if ( ! p -> buffs_chakra_serenity -> check() )
      return false;

    return priest_heal_t::ready();
  }
};

struct holy_word_t : public priest_spell_t
{
  holy_word_sanctuary_t* hw_sanctuary;
  holy_word_chastise_t*  hw_chastise;
  holy_word_serenity_t*  hw_serenity;

  holy_word_t( player_t* player, const std::string& options_str ) :
    priest_spell_t( "holy_word", player, SCHOOL_HOLY, TREE_HOLY ),
    hw_sanctuary( 0 ), hw_chastise( 0 ), hw_serenity( 0 )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    hw_sanctuary = new holy_word_sanctuary_t( p );
    hw_chastise  = new holy_word_chastise_t ( p );
    hw_serenity  = new holy_word_serenity_t ( p );
  }

  virtual void schedule_execute()
  {
    priest_t* p = player -> cast_priest();

    if ( p -> talents.revelations -> rank() && p -> buffs_chakra_serenity -> up() )
    {
      player -> last_foreground_action = hw_serenity;
      hw_serenity -> schedule_execute();
    }

    else if ( p -> talents.revelations -> rank() && p -> buffs_chakra_sanctuary -> up() )
    {
      player -> last_foreground_action = hw_sanctuary;
      hw_sanctuary -> schedule_execute();
    }

    else
    {
      player -> last_foreground_action = hw_chastise;
      hw_chastise -> schedule_execute();
    }
  }

  virtual void execute()
  {
    assert( 0 );
  }

  virtual bool ready()
  {
    priest_t* p = player -> cast_priest();

    if ( p -> talents.revelations -> rank() && p -> buffs_chakra_serenity -> check() )
      return hw_serenity -> ready();

    else if ( p -> talents.revelations -> rank() && p -> buffs_chakra_sanctuary -> check() )
      return hw_sanctuary -> ready();

    else
      return hw_chastise -> ready();
  }
};

// Lightwell Spell ==========================================================

struct lightwell_hot_t : public priest_heal_t
{
  int charges;
  double consume_interval;

  lightwell_hot_t( player_t* player ) :
    priest_heal_t( "lightwell_hot", player, 7001 ),
    charges( 0 ), consume_interval( 0.0 )
  {
    // Hardcoded in the tooltip
    tick_power_mod = 0.308;
    base_multiplier *= 3 * 1.25;

    proc          = true;
    background    = true;
    hasted_ticks  = false;
    may_crit      = false;
  }

  virtual void execute()
  {
    priest_heal_t::execute();

    if ( charges >= 0 )
    {
      // Assuming a Lightwell Charge is used every 10 seconds
      execute_event = new ( sim ) action_execute_event_t( sim, this, consume_interval );
      charges -= 1;
    }
  }
};

struct lightwell_t : public priest_heal_t
{
  lightwell_hot_t* lw_hot;
  double consume_interval;

  lightwell_t( player_t* player, const std::string& options_str ) :
    priest_heal_t( "lightwell", player, 724 ), lw_hot( 0 ), consume_interval( 10.0 )
  {
    option_t options[] =
    {
      { "consume_interval", OPT_FLT, &consume_interval },
      { NULL,     OPT_UNKNOWN, NULL       }
    };
    parse_options( options, options_str );

    assert( consume_interval > 0 && consume_interval < cooldown -> duration );

    priest_t* p = player -> cast_priest();
    check_talent( p -> talents.lightwell -> rank() );

    lw_hot = new lightwell_hot_t( p );

    add_child( lw_hot );
  }

  virtual void execute()
  {
    priest_heal_t::execute();
    priest_t* p = player -> cast_priest();

    lw_hot -> charges = 10;
    lw_hot -> consume_interval = consume_interval;

    lw_hot -> charges += p -> glyphs.lightwell -> effect1().base_value();

    lw_hot -> execute();
  }
};

// Divine Hymn Spell ========================================================

struct divine_hymn_tick_t : public priest_heal_t
{
  int charges;

  divine_hymn_tick_t( player_t* player ) :
    priest_heal_t( "divine_hymn_tick", player, 64844 ),
    charges( 0 )
  {
    background  = true;
    may_crit    = true;
  }

  virtual void execute()
  {
    // Choose Heal Targets
    unsigned int h = 0;
    heal_target.clear();
    heal_target.push_back( find_lowest_player() ); // Find at least the lowest player
    for ( player_t* q = sim -> player_list; q; q = q -> next )
    {
      if( h > 1 ) continue; // This is actually not correct, but since simc partys aren't automatically limited to 5 people it would screw the results on large simulations.
      if ( !q -> is_pet() && q != heal_target[0] )
      {
        heal_target.push_back( q );
        h++;
      }
    }

    priest_heal_t::execute();
  }

  virtual void travel( player_t* t, int travel_result, double travel_dmg )
  {
    priest_heal_t::travel( t, travel_result, travel_dmg );

    charges--;

    if ( charges <= 0 )
      cancel();
  }

  virtual void player_buff()
  {
    priest_heal_t::player_buff();

    priest_t* p = player -> cast_priest();

    if ( p -> buffs_chakra_sanctuary -> up() )
      player_multiplier *= 1.0 + p -> buffs_chakra_sanctuary -> effect1().percent();
  }
};

struct divine_hymn_t : public priest_heal_t
{
  divine_hymn_tick_t* divine_hymn_tick;

  divine_hymn_t( player_t* player, const std::string& options_str ) :
    priest_heal_t( "divine_hymn", player, "Divine Hymn" ),
    divine_hymn_tick( 0 )
  {
    parse_options( NULL, options_str );

    priest_t* p = player -> cast_priest();

    harmful = false;

    channeled = true;

    divine_hymn_tick = new divine_hymn_tick_t( p );

    add_child( divine_hymn_tick );
  }

  virtual void execute()
  {
    divine_hymn_tick -> charges = 12;

    priest_heal_t::execute();
  }

  virtual void tick()
  {
    if ( sim -> debug ) log_t::output( sim, "%s ticks (%d of %d)", name(), dot -> current_tick, dot -> num_ticks );

    if ( divine_hymn_tick -> charges > 0 )
    {
      divine_hymn_tick -> execute();
      stats -> add_tick( time_to_tick );
    }
    else
      cancel();
  }
};

// Tier 12 Healer 2pc Bonus

// Implementation not 100% correct. When the buff expires and is reapplied between 2 events, the event is doubled, which then doubles the mana gain.

// Event
struct tier12_heal_2pc_event_t : public event_t
{
  buff_t* buff;

  tier12_heal_2pc_event_t ( player_t* player,buff_t* b ) :
    event_t( player -> sim, player ), buff( 0 )
  {
    buff = b;
    name = "tier12_heal_2pc";
    sim -> add_event( this, 5.0 );
  }

  virtual void execute()
  {

    if ( buff -> check() )
    {
      priest_t* p = player -> cast_priest();
      player -> resource_gain( RESOURCE_MANA, player -> resource_base[ RESOURCE_MANA ] * 0.02, p -> gains_divine_fire );
      new ( sim ) tier12_heal_2pc_event_t( player, buff );
    }
  }
};

// Buff

struct tier12_heal_2pc_buff_t : public buff_t
{
  tier12_heal_2pc_buff_t( player_t* p, const uint32_t id, const std::string& n, double c ) :
    buff_t ( p, id, n, c )
  {

  }

  virtual void start( int stacks, double value )
  {
    new ( sim ) tier12_heal_2pc_event_t( player, this );
    buff_t::start( stacks, value );

  }
};

} // ANONYMOUS NAMESPACE ====================================================

// ==========================================================================
// Priest Character Definition
// ==========================================================================

// priest_t::empowered_shadows_amount

double priest_t::empowered_shadows_amount() SC_CONST
{
  double a = shadow_orb_amount();

  a += 0.10;

  return a;
}

// priest_t::shadow_orb_amount

double priest_t::shadow_orb_amount() SC_CONST
{
  double a = composite_mastery() * constants.shadow_orb_mastery_value;

  return a;
}

// priest_t::primary_role

int priest_t::primary_role() SC_CONST
{
  if ( player_t::primary_role() == ROLE_HEAL )
    return ROLE_HEAL;

  if ( player_t::primary_role() == ROLE_DPS || player_t::primary_role() == ROLE_SPELL )
    return ROLE_SPELL;

  if ( primary_tree() == TREE_DISCIPLINE || primary_tree() == TREE_HOLY )
    return ROLE_HEAL;

  return ROLE_SPELL;
}

// priest_t::composite_armor =========================================

double priest_t::composite_armor() SC_CONST
{
  double a = player_t::composite_armor();

  if ( buffs_inner_fire -> up() )
    a *= 1.0 + buffs_inner_fire -> base_value( E_APPLY_AURA, A_MOD_RESISTANCE_PCT ) / 100.0 * ( 1.0 + glyphs.inner_fire -> ok() * 0.5 );

  return floor( a );
}

// priest_t::composite_spell_power =========================================

double priest_t::composite_spell_power( const school_type school ) SC_CONST
{
  double sp = player_t::composite_spell_power( school );


  if ( buffs_inner_fire -> up() )
    sp += buffs_inner_fire -> effect_min( 2 );

  return sp;
}

// priest_t::composite_spell_hit =============================================

double priest_t::composite_spell_hit() SC_CONST
{
  double hit = player_t::composite_spell_hit();

  hit += ( ( spirit() - attribute_base[ ATTR_SPIRIT ] ) * constants.twisted_faith_dynamic_value ) / rating.spell_hit;

  return hit;
}

// priest_t::composite_spell_haste =============================================

double priest_t::composite_spell_haste() SC_CONST
{
  double h = player_t::composite_spell_haste();

  h *= constants.darkness_value;
  return h;
}

// priest_t::composite_player_multiplier =========================================

double priest_t::composite_player_multiplier( const school_type school ) SC_CONST
{
  double m = player_t::composite_player_multiplier( school );

  if ( spell_id_t::is_school( school, SCHOOL_SHADOW ) )
  {
    m *= 1.0 + buffs_shadow_form -> check() * constants.shadow_form_value;
    m *= 1.0 + constants.twisted_faith_static_value;
  }
  if ( spell_id_t::is_school( school, SCHOOL_SHADOWLIGHT ) )
  {
    m *= 1.0 + constants.twin_disciplines_value;

    if ( buffs_chakra_chastise -> up() )
    {
      m *= 1.0 + 0.15;
    }
  }
  if ( spell_id_t::is_school( school, SCHOOL_MAGIC ) )
  {
    m *= 1.0 + constants.shadow_power_damage_value;
  }

  return m;
}

double priest_t::composite_player_td_multiplier( const school_type school ) SC_CONST
{
  double player_multiplier = player_t::composite_player_td_multiplier( school );

  if ( school == SCHOOL_SHADOW )
  {
    // Shadow TD
    player_multiplier += buffs_empowered_shadow -> value();
    player_multiplier += buffs_dark_evangelism -> stack () * constants.dark_evangelism_damage_value;
  }

  return player_multiplier;
}

// priest_t::composite_player_heal_multiplier ====================================

double priest_t::composite_player_heal_multiplier( const school_type school ) SC_CONST
{
  double m = player_t::composite_player_heal_multiplier( school );

  if ( spell_id_t::is_school( school, SCHOOL_SHADOWLIGHT ) )
    m *= 1.0 + constants.twin_disciplines_value;

  return m;
}

double priest_t::composite_movement_speed() SC_CONST
{
  double speed = player_t::composite_movement_speed();

  if ( buffs_inner_will -> up() )
    speed *= 1.0 + buffs_inner_will -> effect2().percent() + talents.inner_sanctum -> effect2().percent();

  return speed;
}

double priest_t::matching_gear_multiplier( const attribute_type attr ) SC_CONST
{
  if ( attr == ATTR_INTELLECT )
    return 0.05;

  return 0.0;
}

double priest_t::spirit() SC_CONST
{
  double spi = player_t::spirit();

  if ( set_bonus.tier11_4pc_heal() & ( buffs_chakra_serenity -> up() || buffs_chakra_sanctuary -> up() || buffs_chakra_chastise -> up() ) )
    spi += 540;

  return spi;
}

// priest_t::create_action ===================================================

action_t* priest_t::create_action( const std::string& name,
                                   const std::string& options_str )
{
  // Misc
  if ( name == "dispersion"             ) return new dispersion_t            ( this, options_str );
  if ( name == "fortitude"              ) return new fortitude_t             ( this, options_str );
  if ( name == "inner_fire"             ) return new inner_fire_t            ( this, options_str );
  if ( name == "power_infusion"         ) return new power_infusion_t        ( this, options_str );
  if ( name == "shadow_form"            ) return new shadow_form_t           ( this, options_str );
  if ( name == "vampiric_embrace"       ) return new vampiric_embrace_t      ( this, options_str );
  if ( name == "archangel"              ) return new archangel_t             ( this, options_str );
  if ( name == "chakra"                 ) return new chakra_t                ( this, options_str );
  if ( name == "inner_will"             ) return new inner_will_t            ( this, options_str );
  if ( name == "inner_focus"            ) return new inner_focus_t           ( this, options_str );
  if ( name == "hymn_of_hope"           ) return new hymn_of_hope_t          ( this, options_str );

  // Damage
  if ( name == "devouring_plague"       ) return new devouring_plague_t      ( this, options_str );
  if ( name == "holy_fire"              ) return new holy_fire_t             ( this, options_str );
  if ( name == "mind_blast"             ) return new mind_blast_t            ( this, options_str );
  if ( name == "mind_flay"              ) return new mind_flay_t             ( this, options_str );
  if ( name == "mind_spike"             ) return new mind_spike_t            ( this, options_str );
  if ( name == "mind_sear"              ) return new mind_sear_t             ( this, options_str );
  if ( name == "penance"                ) return new penance_t               ( this, options_str );
  if ( name == "shadow_word_death"      ) return new shadow_word_death_t     ( this, options_str );
  if ( name == "shadow_word_pain"       ) return new shadow_word_pain_t      ( this, options_str );
  if ( name == "smite"                  ) return new smite_t                 ( this, options_str );
  if ( name == "shadow_fiend"           ) return new shadow_fiend_spell_t    ( this, options_str );
  if ( name == "vampiric_touch"         ) return new vampiric_touch_t        ( this, options_str );


  // Heals
  if ( name == "renew"                  ) return new renew_t                 ( this, options_str );
  if ( name == "heal"                   ) return new _heal_t                 ( this, options_str );
  if ( name == "flash_heal"             ) return new flash_heal_t            ( this, options_str );
  if ( name == "binding_heal"           ) return new binding_heal_t          ( this, options_str );
  if ( name == "greater_heal"           ) return new greater_heal_t          ( this, options_str );
  if ( name == "prayer_of_healing"      ) return new prayer_of_healing_t     ( this, options_str );
  if ( name == "circle_of_healing"      ) return new circle_of_healing_t     ( this, options_str );
  if ( name == "prayer_of_mending"      ) return new prayer_of_mending_t     ( this, options_str );
  if ( name == "power_word_shield"      ) return new power_word_shield_t     ( this, options_str );
  if ( name == "penance_heal"           ) return new penance_heal_t          ( this, options_str );
  if ( name == "holy_word"              ) return new holy_word_t             ( this, options_str );
  if ( name == "lightwell"              ) return new lightwell_t             ( this, options_str );
  if ( name == "divine_hymn"            ) return new divine_hymn_t           ( this, options_str );

  return player_t::create_action( name, options_str );
}

// priest_t::create_pet ======================================================

pet_t* priest_t::create_pet( const std::string& pet_name,
                             const std::string& pet_type )
{
  pet_t* p = find_pet( pet_name );

  if ( p ) return p;

  if ( pet_name == "shadow_fiend" ) return new shadow_fiend_pet_t( sim, this );

  return 0;
}

// priest_t::create_pets =====================================================

void priest_t::create_pets()
{
  create_pet( "shadow_fiend" );
}

// priest_t::init_base =======================================================

void priest_t::init_base()
{
  player_t::init_base();

  base_attack_power = -10;
  attribute_multiplier_initial[ ATTR_INTELLECT ]   *= 1.0 + passive_spells.enlightenment -> effect_base_value( 1 ) / 100.0;

  initial_attack_power_per_strength = 1.0;
  initial_spell_power_per_intellect = 1.0;

  mana_per_intellect = 15;

  diminished_kfactor    = 0.009830;
  diminished_dodge_capi = 0.006650;
  diminished_parry_capi = 0.006650;
}

// priest_t::init_gains ======================================================

void priest_t::init_gains()
{
  player_t::init_gains();

  gains_dispersion                = get_gain( "dispersion" );
  gains_shadow_fiend              = get_gain( "shadow_fiend" );
  gains_archangel                 = get_gain( "archangel" );
  gains_masochism                 = get_gain( "masochism" );
  gains_rapture                   = get_gain( "rapture" );
  gains_hymn_of_hope              = get_gain( "hymn_of_hope" );
  gains_divine_fire               = get_gain( "divine_fire" );
}

// priest_t::init_procs ======================================================

void priest_t::init_procs()
{
  player_t::init_procs();

  procs_shadowy_apparation   = get_proc( "shadowy_apparation_proc" );
  procs_surge_of_light       = get_proc( "surge_of_light" );
}

// priest_t::init_scaling ====================================================

void priest_t::init_scaling()
{
  player_t::init_scaling();

  // A Atonement Priest might be Health-caped
  scales_with[ STAT_STAMINA ] = talents.atonement -> ok();

  // For a Shadow Priest Spirit is the same as Hit Rating so invert it.
  if ( ( talents.twisted_faith -> rank() ) && ( sim -> scaling -> scale_stat == STAT_SPIRIT ) )
  {
    double v = sim -> scaling -> scale_value;

    if ( ! sim -> scaling -> positive_scale_delta )
    {
      invert_scaling = 1;
      attribute_initial[ ATTR_SPIRIT ] -= v * 2;
    }
  }

}

// priest_t::init_uptimes ====================================================

void priest_t::init_uptimes()
{
  player_t::init_uptimes();

  uptimes_mind_spike[ 0 ]  = get_uptime( "mind_spike_0" );
  uptimes_mind_spike[ 1 ]  = get_uptime( "mind_spike_1" );
  uptimes_mind_spike[ 2 ]  = get_uptime( "mind_spike_2" );
  uptimes_mind_spike[ 3 ]  = get_uptime( "mind_spike_3" );

  uptimes_dark_flames      = get_uptime( "dark_flames" );

  uptimes_dark_evangelism[ 0 ]  = get_uptime( "dark_evangelism_0" );
  uptimes_dark_evangelism[ 1 ]  = get_uptime( "dark_evangelism_1" );
  uptimes_dark_evangelism[ 2 ]  = get_uptime( "dark_evangelism_2" );
  uptimes_dark_evangelism[ 3 ]  = get_uptime( "dark_evangelism_3" );
  uptimes_dark_evangelism[ 4 ]  = get_uptime( "dark_evangelism_4" );
  uptimes_dark_evangelism[ 5 ]  = get_uptime( "dark_evangelism_5" );

  uptimes_holy_evangelism[ 0 ]  = get_uptime( "holy_evangelism_0" );
  uptimes_holy_evangelism[ 1 ]  = get_uptime( "holy_evangelism_1" );
  uptimes_holy_evangelism[ 2 ]  = get_uptime( "holy_evangelism_2" );
  uptimes_holy_evangelism[ 3 ]  = get_uptime( "holy_evangelism_3" );
  uptimes_holy_evangelism[ 4 ]  = get_uptime( "holy_evangelism_4" );
  uptimes_holy_evangelism[ 5 ]  = get_uptime( "holy_evangelism_5" );

  uptimes_shadow_orb[ 0 ]  = get_uptime( "shadow_orb_0" );
  uptimes_shadow_orb[ 1 ]  = get_uptime( "shadow_orb_1" );
  uptimes_shadow_orb[ 2 ]  = get_uptime( "shadow_orb_2" );
  uptimes_shadow_orb[ 3 ]  = get_uptime( "shadow_orb_3" );

  uptimes_test_of_faith = get_uptime( "test_of_faith" );
}

// priest_t::init_rng ========================================================

void priest_t::init_rng()
{
  player_t::init_rng();

  rng_pain_and_suffering = get_rng( "pain_and_suffering" );
}

// priest_t::init_talents
void priest_t::init_talents()
{
  // Discipline
  talents.improved_power_word_shield  = find_talent( "Improved Power Word: Shield" );
  talents.twin_disciplines            = find_talent( "Twin Disciplines" );
  talents.mental_agility              = find_talent( "Mental Agility" );
  talents.evangelism                  = find_talent( "Evangelism" );
  talents.archangel                   = find_talent( "Archangel" );
  talents.inner_sanctum               = find_talent( "Inner Sanctum" );
  talents.soul_warding                = find_talent( "Soul Warding" );
  talents.renewed_hope                = find_talent( "Renewed Hope" );
  talents.power_infusion              = find_talent( "Power Infusion" );
  talents.atonement                   = find_talent( "Atonement" );
  talents.inner_focus                 = find_talent( "Inner Focus" );
  talents.rapture                     = find_talent( "Rapture" );
  talents.borrowed_time               = find_talent( "Borrowed Time" );
  talents.strength_of_soul            = find_talent( "Strength of Soul" );
  talents.divine_aegis                = find_talent( "Divine Aegis" );
  // Pain Supression
  talents.train_of_thought            = find_talent( "Train of Thought" );
  talents.grace                       = find_talent( "Grace" );
  talents.power_word_barrier          = find_talent( "Power Word: Barrier" );


  // Holy
  talents.improved_renew              = find_talent( "Improved Renew" );
  talents.empowered_healing           = find_talent( "Empowered Healing" );
  talents.divine_fury                 = find_talent( "Divine Fury" );
  // Desperate Prayer
  talents.surge_of_light              = find_talent( "Surge of Light" );
  talents.inspiration                 = find_talent( "Inspiration" );
  talents.divine_touch                = find_talent( "Divine Touch" );
  talents.holy_concentration          = find_talent( "Holy Concentration" );
  talents.lightwell                   = find_talent( "Lightwell" );
  talents.tome_of_light               = find_talent( "Tome of Light" );
  talents.rapid_renewal               = find_talent( "Rapid Renewal" );
  talents.serendipity                 = find_talent( "Serendipity" );
  talents.body_and_soul               = find_talent( "Body and Soul" );
  talents.chakra                      = find_talent( "Chakra" );
  talents.revelations                 = find_talent( "Revelations" );
  talents.test_of_faith               = find_talent( "Test of Faith" );
  talents.state_of_mind               = find_talent( "State of Mind" );
  talents.circle_of_healing           = find_talent( "Circle of Healing" );


  // Shadow
  talents.darkness                    = find_talent( "Darkness" );
  talents.improved_devouring_plague   = find_talent( "Improved Devouring Plague" );
  talents.improved_mind_blast         = find_talent( "Improved Mind Blast" );
  talents.mind_melt                   = find_talent( "Mind Melt" );
  talents.dispersion                  = find_talent( "Dispersion" );
  talents.improved_shadow_word_pain   = find_talent( "Improved Shadow Word: Pain" );
  talents.pain_and_suffering          = find_talent( "Pain and Suffering" );
  talents.masochism                   = find_talent( "Masochism" );
  talents.shadow_form                 = find_talent( "Shadowform" );
  talents.twisted_faith               = find_talent( "Twisted Faith" );
  talents.veiled_shadows              = find_talent( "Veiled Shadows" );
  talents.harnessed_shadows           = find_talent( "Harnessed Shadows" );
  talents.shadowy_apparition          = find_talent( "Shadowy Apparition" );
  talents.vampiric_embrace            = find_talent( "Vampiric Embrace" );
  talents.vampiric_touch              = find_talent( "Vampiric Touch" );
  talents.sin_and_punishment          = find_talent( "Sin and Punishment" );
  talents.paralysis                   = find_talent( "Paralysis" );
  talents.phantasm                    = find_talent( "Phantasm" );

  player_t::init_talents();
}

// priest_t::init_spells
void priest_t::init_spells()
{
  player_t::init_spells();

  // Passive Spells

  // Discipline
  passive_spells.enlightenment          = new passive_spell_t( this, "enlightenment", "Enlightenment" );
  passive_spells.meditation_disc        = new passive_spell_t( this, "meditation_disc", "Meditation" );
  passive_spells.dark_evangelism_1      = new passive_spell_t( this, "dark_evangelism_1", 87117 );
  passive_spells.dark_evangelism_2      = new passive_spell_t( this, "dark_evangelism_2", 87118 );
  passive_spells.holy_evangelism_1      = new passive_spell_t( this, "holy_evangelism_1", 81660 );
  passive_spells.holy_evangelism_2      = new passive_spell_t( this, "holy_evangelism_2", 81661 );

  // Holy
  passive_spells.spiritual_healing      = new passive_spell_t( this, "spiritual_healing", "Enlightenment" );
  passive_spells.meditation_holy        = new passive_spell_t( this, "meditation_holy", 95861 );

  // Shadow
  passive_spells.shadow_power           = new passive_spell_t( this, "shadow_power", "Shadow Power" );
  passive_spells.shadow_orbs            = new passive_spell_t( this, "shadow_orbs", "Shadow Orbs" );
  passive_spells.shadowy_apparition_num = new passive_spell_t( this, "shadowy_apparition_num", 78202 );



  // Mastery Spells
  mastery_spells.shield_discipline = new mastery_t( this, "shield_discipline", "Shield Discipline", TREE_DISCIPLINE );
  mastery_spells.echo_of_light     = new mastery_t( this, "echo_of_light", "Echo of Light", TREE_HOLY );
  mastery_spells.shadow_orb_power  = new mastery_t( this, "shadow_orb_power", "Shadow Orb Power", TREE_SHADOW );



  // Active Spells
  active_spells.mind_spike      = new active_spell_t( this, "mind_spike", "Mind Spike" );
  active_spells.shadow_fiend    = new active_spell_t( this, "shadow_fiend", "Shadowfiend" );
  active_spells.holy_archangel  = new active_spell_t( this, "holy_archangel", 87152 );
  active_spells.holy_archangel2 = new active_spell_t( this, "holy_archangel2", 81700 );
  active_spells.dark_archangel  = new active_spell_t( this, "dark_archangel", 87153 );

  dark_flames                   = spell_data_t::find( 99158, "Dark Flames", dbc.ptr );



  // Glyphs
  glyphs.dispersion         = find_glyph( "Glyph of Dispersion" );
  glyphs.divine_accuracy    = find_glyph( "Glyph of Divine Accuracy" );
  glyphs.holy_nova          = find_glyph( "Glyph of Holy Nova" );
  glyphs.inner_fire         = find_glyph( "Glyph of Inner Fire" );
  glyphs.mind_flay          = find_glyph( "Glyph of Mind Flay" );
  glyphs.penance            = find_glyph( "Glyph of Penance" );
  glyphs.spirit_tap         = find_glyph( "Glyph of Spirit Tap" );
  glyphs.shadow_word_death  = find_glyph( "Glyph of Shadow Word: Death" );
  glyphs.shadow_word_pain   = find_glyph( "Glyph of Shadow Word: Pain" );
  glyphs.smite              = find_glyph( "Glyph of Smite" );
  glyphs.renew              = find_glyph( "Glyph of Renew" );
  glyphs.power_word_shield  = find_glyph( "Glyph of Power Word: Shield" );
  glyphs.prayer_of_healing  = find_glyph( "Glyph of Prayer of Healing" );
  glyphs.lightwell          = find_glyph( "Glyph of Lightwell" );
  glyphs.circle_of_healing  = find_glyph( "Glyph of Circle of Healing" );
  glyphs.prayer_of_mending  = find_glyph( "Glyph of Prayer of Mending" );


  // Set Bonuses
  // T11: H2P = 89910, H4P = 89911
  static uint32_t set_bonuses[N_TIER][N_TIER_BONUS] =
  {
    //  C2P    C4P    M2P    M4P    T2P    T4P    H2P    H4P
    { 89915, 89922,     0,     0,     0,     0,     0,     0 }, // Tier11
    { 99154, 99157,     0,     0,     0,     0, 99134, 99135 }, // Tier12
    {     0,     0,     0,     0,     0,     0,     0,     0 },
  };

  sets = new set_bonus_array_t( this, set_bonuses );
}

// priest_t::init_buffs ======================================================

void priest_t::init_buffs()
{
  player_t::init_buffs();


  // Discipline
  buffs_borrowed_time              = new buff_t( this, talents.borrowed_time -> effect_trigger_spell( 1 ), "borrowed_time", talents.borrowed_time -> rank() );
  buffs_dark_evangelism            = new buff_t( this, 81661, "dark_evangelism" );
  buffs_holy_evangelism            = new buff_t( this, 81661, "holy_evangelism" );
  buffs_dark_archangel             = new buff_t( this, 87153, "dark_archangel" );
  buffs_holy_archangel             = new buff_t( this, 81700, "holy_archangel" );
  buffs_inner_fire                 = new buff_t( this,   588, "inner_fire"                                              );
  buffs_inner_focus                = new buff_t( this, "inner_focus", "Inner Focus" );
  buffs_inner_focus -> cooldown -> duration = 0;
  buffs_inner_will                 = new buff_t( this, "inner_will", "Inner Will"                                );
  buffs_weakened_soul              = new buff_t( this,  6788, "weakened_soul" );

  // Holy
  buffs_chakra_pre                 = new buff_t( this, 14751, "chakra_pre" );
  buffs_chakra_chastise            = new buff_t( this, 81209, "chakra_chastise" );
  buffs_chakra_sanctuary           = new buff_t( this, 81206, "chakra_sanctuary" );
  buffs_chakra_serenity            = new buff_t( this, 81208, "chakra_serenity" );
  buffs_serendipity                = new buff_t( this, talents.serendipity -> effect_trigger_spell( 1 ), "serendipity", talents.serendipity -> rank() );
  buffs_serenity                   = new buff_t( this, 88684, "chakra_serenity_crit" );
  buffs_surge_of_light             = new buff_t( this, talents.surge_of_light, NULL );


  // Shadow
  buffs_empowered_shadow           = new buff_t( this, 95799, "empowered_shadow" );
  buffs_glyph_of_shadow_word_death = new buff_t( this, "glyph_of_shadow_word_death", 1, 6.0                      );
  buffs_mind_melt                  = new buff_t( this, "mind_melt"                 );
  buffs_mind_spike                 = new buff_t( this, "mind_spike",                 3, 12.0                     );
  buffs_shadow_form                = new buff_t( this, "shadow_form",                1                           );
  buffs_shadow_orb                 = new buff_t( this, 77487, "shadow_orb" );
  buffs_shadowfiend                = new buff_t( this, "shadowfiend",                1                           );
  buffs_spirit_tap                 = new buff_t( this, "spirit_tap",                 1, 12.0                     );
  buffs_vampiric_embrace           = new buff_t( this, "vampiric_embrace",           1                           );

  // Set Bonus
  buffs_indulgence_of_the_penitent = new buff_t( this, 89913, "indulgence_of_the_penitent", set_bonus.tier11_4pc_heal() );
  buffs_divine_fire = new tier12_heal_2pc_buff_t( this, 99132, "divine_fire", set_bonus.tier12_2pc_heal() );
}

// priest_t::init_actions =====================================================

void priest_t::init_actions()
{
  if ( action_list_str.empty() )
  {
    if ( level > 80 )
    {
      action_list_str  = "flask,type=draconic_mind/food,type=seafood_magnifique_feast";
    }
    else if ( level >= 75 )
    {
      action_list_str  = "flask,type=frost_wyrm/food,type=fish_feast";
    }

    action_list_str += "/fortitude/inner_fire";

    if ( talents.shadow_form -> rank() ) action_list_str += "/shadow_form";

    if ( talents.vampiric_embrace -> rank() ) action_list_str += "/vampiric_embrace";

    action_list_str += "/snapshot_stats";

    int num_items = ( int ) items.size();
    for ( int i=0; i < num_items; i++ )
    {
      if ( items[ i ].use.active() )
      {
        action_list_str += "/use_item,name=";
        action_list_str += items[ i ].name();
      }
    }

    // Lifeblood
    if ( profession[ PROF_HERBALISM ] >= 450 )
      action_list_str += "/lifeblood";

    switch ( primary_tree() )
    {
      // SHADOW
    case TREE_SHADOW:
      if ( level > 80 )
      {
                                                         action_list_str += "/volcanic_potion,if=!in_combat";
                                                         action_list_str += "/volcanic_potion,if=buff.bloodlust.react|target.time_to_die<=40";
      }
      else if ( level >= 70 )
      {
                                                         action_list_str += "/wild_magic_potion,if=!in_combat";
                                                         action_list_str += "/speed_potion,if=buff.bloodlust.react|target.time_to_die<=20";
      }

                                                         action_list_str += "/mind_blast,if=buff.shadow_orb.stack>=1";
      if ( dbc.ptr )                                     action_list_str += "&!set_bonus.tier12_4pc_caster";

      if ( race == RACE_TROLL )                          action_list_str += "/berserking";
      if ( race == RACE_BLOOD_ELF )                      action_list_str += "/arcane_torrent";
                                                         action_list_str += "/shadow_word_pain,if=(!ticking|dot.shadow_word_pain.remains<gcd+0.5)&miss_react";
      if ( level >= 28 )                                 action_list_str += "/devouring_plague,if=(!ticking|dot.devouring_plague.remains<gcd+1.0)&miss_react";

                                                         action_list_str += "/stop_moving,health_percentage<=25,if=cooldown.shadow_word_death.remains>=0.2";
      if ( talents.vampiric_touch -> rank() )            action_list_str += "|dot.vampiric_touch.remains<cast_time+2.5";
      if ( talents.vampiric_touch -> rank() )            action_list_str += "/vampiric_touch,if=(!ticking|dot.vampiric_touch.remains<cast_time+2.5)&miss_react";
                                                         
      if ( talents.archangel -> ok() )
      {
                                                         action_list_str += "/archangel,if=buff.dark_evangelism.stack>=5";
        if ( talents.vampiric_touch -> rank() )          action_list_str += "&dot.vampiric_touch.remains>5";
                                                         action_list_str += "&dot.devouring_plague.remains>5";
      }

      if ( dbc.ptr )
      {
                                                         action_list_str += "/mind_blast,if=buff.shadow_orb.stack>=1";
                                                         action_list_str += "&set_bonus.tier12_4pc_caster";
      }
                    
                                                         action_list_str += "/start_moving,health_percentage<=25,if=cooldown.shadow_word_death.remains<=0.1";

                                                         action_list_str += "/shadow_word_death,health_percentage<=25";
      if ( level >= 66 )                                 action_list_str += "/shadow_fiend";
                                                         action_list_str += "/mind_blast";
                                                         action_list_str += "/mind_flay";
      if ( talents.dispersion -> rank() )                action_list_str += "/dispersion,moving=1";
      if ( talents.improved_devouring_plague -> rank() ) action_list_str += "/devouring_plague,moving=1,if=mana_pct>10";
                                                         action_list_str += "/shadow_word_death,moving=1";
      if ( talents.dispersion -> rank() )                action_list_str += "/dispersion";
      break;


      // DISCIPLINE
    case TREE_DISCIPLINE:
      // DAMAGE DEALER
      if ( primary_role() != ROLE_HEAL )
      {
                                                         action_list_str += "/mana_potion,if=mana_pct<=75";
        if ( level >= 66 )                               action_list_str += "/shadow_fiend,if=mana_pct<=50";
        if ( level >= 64 )                               action_list_str += "/hymn_of_hope";
        if ( level >= 66 )                               action_list_str += ",if=pet.shadow_fiend.active&time>200";
        if ( race == RACE_TROLL )                        action_list_str += "/berserking";
        if ( race == RACE_BLOOD_ELF )                    action_list_str += "/arcane_torrent,if=mana_pct<=90";
        if ( talents.power_infusion -> rank() )          action_list_str += "/power_infusion";
        if ( talents.archangel -> ok() )                 action_list_str += "/archangel,if=buff.holy_evangelism.stack>=5";
        if ( talents.rapture -> ok() )                   action_list_str += "/power_word_shield,if=buff.weakened_soul.down";
                                                         action_list_str += "/holy_fire";
        if ( level >= 28 )                               action_list_str += "/devouring_plague,if=remains<tick_time|!ticking";
                                                         action_list_str += "/shadow_word_pain,if=remains<tick_time|!ticking";
                                                         action_list_str += "/penance";
        if ( ! talents.archangel -> ok() )               action_list_str += "/mind_blast";
                                                         action_list_str += "/smite";
        if ( talents.archangel -> ok() )                 action_list_str += ",if=buff.holy_evangelism.stack<5|buff.holy_evangelism.remains<cooldown.archangel.remains+0.5";
      }
      // HEALER
      else
      {
                                                         action_list_str += "/mana_potion,if=mana_pct<=75";
        if ( race == RACE_BLOOD_ELF )                    action_list_str += "/arcane_torrent,if=mana_pct<=90";
        if ( level >= 66 )                               action_list_str += "/shadow_fiend,if=mana_pct<=20";
        if ( level >= 64 )                               action_list_str += "/hymn_of_hope";
        if ( level >= 66 )                               action_list_str += ",if=pet.shadow_fiend.active";
        if ( talents.archangel -> ok() )                 action_list_str += "/archangel,if=buff.holy_evangelism.stack>=5";
        if ( race == RACE_TROLL )                        action_list_str += "/berserking";
      }
      break;

      // HOLY
    case TREE_HOLY:
      // DAMAGE DEALER
      if ( primary_role() != ROLE_HEAL )
      {
                                                         action_list_str += "/mana_potion,if=mana_pct<=75";
        if ( level >= 66 )                               action_list_str += "/shadow_fiend,if=mana_pct<=50";
        if ( level >= 64 )                               action_list_str += "/hymn_of_hope";
        if ( level >= 66 )                               action_list_str += ",if=pet.shadow_fiend.active&time>200";
        if ( race == RACE_TROLL )                        action_list_str += "/berserking";
        if ( race == RACE_BLOOD_ELF )                    action_list_str += "/arcane_torrent,if=mana_pct<=90";
        if ( talents.chakra -> ok() )                    action_list_str += "/chakra";
        if ( talents.archangel -> ok() )                 action_list_str += "/archangel,if=buff.holy_evangelism.stack>=5";
                                                         action_list_str += "/holy_fire";
        if ( level >= 28 )                               action_list_str += "/devouring_plague,if=remains<tick_time|!ticking";
                                                         action_list_str += "/shadow_word_pain,if=remains<tick_time|!ticking";
        if ( ! talents.archangel -> ok() )               action_list_str += "/mind_blast";
                                                         action_list_str += "/smite";
        if ( talents.archangel -> ok() )                 action_list_str += ",if=buff.holy_evangelism.stack<5|buff.holy_evangelism.remains<cooldown.archangel.remains+0.5";
      }
      // HEALER
      else
      {
                                                         action_list_str += "/mana_potion,if=mana_pct<=75";
        if ( race == RACE_BLOOD_ELF )                    action_list_str += "/arcane_torrent,if=mana_pct<80";
        if ( level >= 66 )                               action_list_str += "/shadow_fiend,if=mana_pct<=20";
        if ( level >= 64 )                               action_list_str += "/hymn_of_hope";
        if ( level >= 66 )                               action_list_str += ",if=pet.shadow_fiend.active";
        if ( race == RACE_TROLL )                        action_list_str += "/berserking";
      }
      break;
    default:
                                                         action_list_str += "/mana_potion,if=mana_pct<=75";
      if ( level >= 66 )                                 action_list_str += "/shadow_fiend,if=mana_pct<=50";
      if ( level >= 64 )                                 action_list_str += "/hymn_of_hope";
      if ( level >= 66 )                                 action_list_str += ",if=pet.shadow_fiend.active&time>200";
      if ( race == RACE_TROLL )                          action_list_str += "/berserking";
      if ( race == RACE_BLOOD_ELF )                      action_list_str += "/arcane_torrent,if=mana_pct<=90";
      if ( talents.archangel -> ok() )                   action_list_str += "/archangel,if=buff.holy_evangelism.stack>=5";
                                                         action_list_str += "/holy_fire";
      if ( level >= 28 )                                 action_list_str += "/devouring_plague,if=remains<tick_time|!ticking";
                                                         action_list_str += "/shadow_word_pain,if=remains<tick_time|!ticking";
                                                         action_list_str += "/mind_blast";
                                                         action_list_str += "/smite";
      break;
    }

    action_list_default = 1;
  }

  player_t::init_actions();

  for( action_t* a = action_list; a; a = a -> next )
  {
    double c = a -> cost();
    if ( c > max_mana_cost ) max_mana_cost = c;
  }
}

// priest_t::init_party ======================================================

void priest_t::init_party()
{
  party_list.clear();

  if ( party == 0 )
    return;

  player_t* p = sim -> player_list;
  while ( p )
  {
    if ( ( p != this ) && ( p -> party == party ) && ( ! p -> quiet ) && ( ! p -> is_pet() ) )
    {
      party_list.push_back( p );
    }
    p = p -> next;
  }
}

// priest_t::init_values =====================================================

void priest_t::init_values()
{
  player_t::init_values();

  // Discipline/Holy
  constants.meditation_value                = passive_spells.meditation_disc    -> ok() ?
                                              passive_spells.meditation_disc  -> base_value( E_APPLY_AURA, A_MOD_MANA_REGEN_INTERRUPT ) :
                                              passive_spells.meditation_holy  -> base_value( E_APPLY_AURA, A_MOD_MANA_REGEN_INTERRUPT );

  // Discipline
  constants.twin_disciplines_value          = talents.twin_disciplines->base_value( E_APPLY_AURA, A_MOD_DAMAGE_PERCENT_DONE );

  switch ( talents.evangelism -> rank() )
  {
  case 1:
    constants.dark_evangelism_damage_value  = passive_spells.dark_evangelism_1  -> base_value( E_APPLY_AURA, A_ADD_PCT_MODIFIER, 22 );
    constants.holy_evangelism_damage_value  = passive_spells.holy_evangelism_1  -> base_value( E_APPLY_AURA, A_ADD_PCT_MODIFIER, 0 );
    constants.holy_evangelism_mana_value    = passive_spells.holy_evangelism_1  -> base_value( E_APPLY_AURA, A_ADD_PCT_MODIFIER, 14 );
    break;
  case 2:
    constants.dark_evangelism_damage_value  = passive_spells.dark_evangelism_2  -> base_value( E_APPLY_AURA, A_ADD_PCT_MODIFIER, 22 );
    constants.holy_evangelism_damage_value  = passive_spells.holy_evangelism_2  -> base_value( E_APPLY_AURA, A_ADD_PCT_MODIFIER, 0 );
    constants.holy_evangelism_mana_value    = passive_spells.holy_evangelism_2  -> base_value( E_APPLY_AURA, A_ADD_PCT_MODIFIER, 14 );
    break;
  default:
    constants.dark_evangelism_damage_value  = 0.0;
    constants.holy_evangelism_damage_value  = 0.0;
    constants.holy_evangelism_mana_value    = 0.0;
    break;
  }


  constants.dark_archangel_damage_value   = active_spells.dark_archangel   -> base_value( E_APPLY_AURA, A_ADD_PCT_MODIFIER, 22 );
  constants.dark_archangel_mana_value     = active_spells.dark_archangel   -> effect_base_value( 3 ) / 100.0;
  constants.holy_archangel_value          = active_spells.holy_archangel2  -> base_value( E_APPLY_AURA, A_MOD_HEALING_DONE_PERCENT ) / 100.0;
  constants.archangel_mana_value          = active_spells.holy_archangel   -> base_value( E_ENERGIZE_PCT );

  // Holy
  constants.holy_concentration_value        = talents.holy_concentration        -> effect1().percent();

  // Shadow Core
  constants.shadow_power_damage_value       = passive_spells.shadow_power       -> effect_base_value( 1 ) / 100.0;
  // FIX-ME
  // Hotfix nerf to Shadow Power
  constants.shadow_power_damage_value       = passive_spells.shadow_power -> ok() ? 0.15 : 0;

  constants.shadow_power_crit_value         = passive_spells.shadow_power       -> effect_base_value( 2 ) / 100.0;
  constants.shadow_orb_proc_value           = mastery_spells.shadow_orb_power   -> proc_chance();
  constants.shadow_orb_mastery_value        = mastery_spells.shadow_orb_power   -> base_value( E_APPLY_AURA, A_DUMMY, P_GENERIC );

  // Shadow
  constants.darkness_value                  = 1.0 / ( 1.0 + talents.darkness    -> effect1().percent() );
  constants.improved_shadow_word_pain_value = talents.improved_shadow_word_pain -> effect1().percent();
  constants.twisted_faith_static_value      = talents.twisted_faith             -> effect2().percent();
  constants.twisted_faith_dynamic_value     = talents.twisted_faith             -> effect1().percent();
  constants.shadow_form_value               = talents.shadow_form               -> effect2().percent();
  constants.harnessed_shadows_value         = talents.harnessed_shadows         -> effect1().percent();
  constants.pain_and_suffering_value        = talents.pain_and_suffering        -> proc_chance();
  constants.devouring_plague_health_mod     = 0.15;

  cooldowns_shadow_fiend -> duration        = active_spells.shadow_fiend        -> cooldown() +
                                              talents.veiled_shadows            -> effect2().seconds() +
                                              ( set_bonus.tier12_2pc_caster() ? -75.0 : 0.0 );

  constants.max_shadowy_apparitions         = passive_spells.shadowy_apparition_num -> effect_base_value( 1 );
}


// priest_t::reset ===========================================================

void priest_t::reset()
{
  player_t::reset();

  while ( shadowy_apparition_active_list.size() )
  {
    spell_t* s = shadowy_apparition_active_list.front();

    shadowy_apparition_active_list.pop_front();

    shadowy_apparition_free_list.push( s );
  }

  priest_spell_t::add_more_shadowy_apparitions( this );

  recast_mind_blast = 0;

  echo_of_light_merged = false;

  if ( talents.atonement -> rank() )
  {
    if ( ! atonement_nc )
      atonement_nc = new atonement_nc_t( this );
    if ( ! atonement_c )
      atonement_c = new atonement_c_t( this );
  }

  was_sub_25 = false;

  mana_resource.reset();

  heals_echo_of_light                  = 0;


  init_party();
}

// priest_t::regen  ==========================================================

void priest_t::regen( double periodicity )
{
  mana_regen_while_casting = constants.meditation_value + constants.holy_concentration_value;

  player_t::regen( periodicity );
}

// priest_t::create_expression =================================================

action_expr_t* priest_t::create_expression( action_t* a, const std::string& name_str )
{
  if ( name_str == "recast_mind_blast" )
  {
    struct recast_mind_blast_expr_t : public action_expr_t
    {
      recast_mind_blast_expr_t( action_t* a ) : action_expr_t( a, "recast_mind_blast" ) { result_type = TOK_NUM; }
      virtual int evaluate() { result_num = action -> player -> cast_priest() -> recast_mind_blast; return TOK_NUM; }
    };
    return new recast_mind_blast_expr_t( a );
  }
  if ( name_str == "use_mind_blast" )
  {
    struct use_mind_blast_expr_t : public action_expr_t
    {
      use_mind_blast_expr_t( action_t* a ) : action_expr_t( a, "use_mind_blast" ) { result_type = TOK_NUM; }
      virtual int evaluate() { result_num = action -> player -> cast_priest() -> use_mind_blast; return TOK_NUM; }
    };
    return new use_mind_blast_expr_t( a );
  }
  if ( name_str == "use_shadow_word_death" )
  {
    struct use_shadow_word_death_expr_t : public action_expr_t
    {
      use_shadow_word_death_expr_t( action_t* a ) : action_expr_t( a, "use_shadow_word_death" ) { result_type = TOK_NUM; }
      virtual int evaluate() { result_num = action -> player -> cast_priest() -> use_shadow_word_death; return TOK_NUM; }
    };
    return new use_shadow_word_death_expr_t( a );
  }

  return player_t::create_expression( a, name_str );
}

// priest_t::resource_gain ===================================================

double priest_t::resource_gain( int       resource,
                                double    amount,
                                gain_t*   source,
                                action_t* action )
{
  double actual_amount = player_t::resource_gain( resource, amount, source, action );

  if ( resource == RESOURCE_MANA )
  {
    if ( source != gains_shadow_fiend &&
         source != gains_dispersion )
    {
      mana_resource.mana_gain += actual_amount;
    }
  }

  return actual_amount;
}

// priest_t::resource_loss ===================================================

double priest_t::resource_loss( int       resource,
                                double    amount,
                                action_t* action )
{
  double actual_amount = player_t::resource_loss( resource, amount, action );


  if ( resource == RESOURCE_MANA )
  {
    mana_resource.mana_loss += actual_amount;
  }

  return actual_amount;
}

// priest_t::target_mitigation =============================================

double priest_t::target_mitigation( double            amount,
                                    const school_type school,
                                    int               dmg_type,
                                    int               result,
                                    action_t*         action )
{
  amount = player_t::target_mitigation( amount, school, dmg_type, result, action );

  if ( buffs_shadow_form -> check() )
  {
    amount *= 0.85;
  }
  if ( talents.inner_sanctum -> rank() )
  {
    amount *= 1.0 - talents.inner_sanctum -> rank() * 0.02;
  }

  return amount;
}

// priest_t::create_options ================================================

void priest_t::create_options()
{
  player_t::create_options();

  option_t priest_options[] =
  {
    { "use_shadow_word_death",   OPT_BOOL,   &( use_shadow_word_death     ) },
    { "use_mind_blast",          OPT_INT,    &( use_mind_blast            ) },
    { "power_infusion_target",   OPT_STRING, &( power_infusion_target_str ) },

    { NULL, OPT_UNKNOWN, NULL }
  };

  option_t::copy( options, priest_options );
}

// priest_t::create_profile ===================================================

bool priest_t::create_profile( std::string& profile_str, int save_type, bool save_html )
{
  player_t::create_profile( profile_str, save_type, save_html );

  if ( save_type == SAVE_ALL )
  {
    std::string temp_str;
    if ( ! power_infusion_target_str.empty() ) profile_str += "power_infusion_target=" + power_infusion_target_str + "\n";
    if ( ( use_shadow_word_death ) || ( use_mind_blast != 1 ) )
    {
      profile_str += "## Variables\n";
    }
    if ( use_shadow_word_death )
    {
      profile_str += "use_shadow_word_death=1\n";
    }
    if ( use_mind_blast != 1 )
    {
      temp_str = util_t::to_string( use_mind_blast );
      profile_str += "use_mind_blast=" + temp_str + "\n";
    }
  }

  return true;
}

// priest_t::copy_from ===================================================

void priest_t::copy_from( player_t* source )
{
  player_t::copy_from( source );

  priest_t* p = source -> cast_priest();

  power_infusion_target_str = p -> power_infusion_target_str;
  use_shadow_word_death     = p -> use_shadow_word_death;
  use_mind_blast            = p -> use_mind_blast;
}

// priest_t::decode_set =====================================================

int priest_t::decode_set( item_t& item )
{
  if ( item.slot != SLOT_HEAD      &&
       item.slot != SLOT_SHOULDERS &&
       item.slot != SLOT_CHEST     &&
       item.slot != SLOT_HANDS     &&
       item.slot != SLOT_LEGS      )
  {
    return SET_NONE;
  }

  const char* s = item.name();

  bool is_caster = false;
  bool is_healer = false;

  if ( strstr( s, "mercurial" ) )
  {
    is_caster = ( strstr( s, "hood"          ) ||
                  strstr( s, "shoulderwraps" ) ||
                  strstr( s, "vestment"      ) ||
                  strstr( s, "gloves"        ) ||
                  strstr( s, "leggings"      ) );
    if ( is_caster ) return SET_T11_CASTER;

    is_healer = ( strstr( s, "cowl"          ) ||
                  strstr( s, "mantle"        ) ||
                  strstr( s, "robes"         ) ||
                  strstr( s, "handwraps"     ) ||
                  strstr( s, "legwraps"      ) );
    if ( is_healer ) return SET_T11_HEAL;
  }

  if ( strstr( s, "_of_the_cleansing_flame" ) )
  {
    is_caster = ( strstr( s, "hood"          ) ||
                  strstr( s, "shoulderwraps" ) ||
                  strstr( s, "vestment"      ) ||
                  strstr( s, "gloves"        ) ||
                  strstr( s, "leggings"      ) );
    if ( is_caster ) return SET_T12_CASTER;

    is_healer = ( strstr( s, "cowl"          ) ||
                  strstr( s, "mantle"        ) ||
                  strstr( s, "robes"         ) ||
                  strstr( s, "handwraps"     ) ||
                  strstr( s, "legwraps"      ) );
    if ( is_healer ) return SET_T12_HEAL;
  }

  return SET_NONE;
}

// ==========================================================================
// PLAYER_T EXTENSIONS
// ==========================================================================

// player_t::create_priest  =================================================

player_t* player_t::create_priest( sim_t* sim, const std::string& name, race_type r )
{
  return new priest_t( sim, name, r );
}

// player_t::priest_init =====================================================

void player_t::priest_init( sim_t* sim )
{
  sim -> auras.mind_quickening = new aura_t( sim, "mind_quickening", 1, 0.0 );

  for ( unsigned int i = 0; i < sim -> actor_list.size(); i++ )
  {
    player_t* p = sim -> actor_list[i];
    p -> buffs.fortitude      = new stat_buff_t( p, "fortitude",       STAT_STAMINA, floor( sim -> dbc.effect_average( sim -> dbc.spell( 79104 ) -> effect1().id(), sim -> max_player_level ) ), !p -> is_pet() );
    p -> buffs.power_infusion = new      buff_t( p, "power_infusion",             1,  15.0, 0 );
    p -> buffs.inspiration    = new      buff_t( p, "inspiration", 1, 15.0, 0 );
  }

}

// player_t::priest_combat_begin =============================================

void player_t::priest_combat_begin( sim_t* sim )
{
  if ( sim -> overrides.mind_quickening ) sim -> auras.mind_quickening -> override();

  for ( player_t* p = sim -> player_list; p; p = p -> next )
  {
    if ( p -> ooc_buffs() )
    {
      if ( sim -> overrides.fortitude     ) p -> buffs.fortitude     -> override( 1, floor( sim -> dbc.effect_average( sim -> dbc.spell( 79104 ) -> effect1().id(), sim -> max_player_level ) ) );
    }
  }
}
