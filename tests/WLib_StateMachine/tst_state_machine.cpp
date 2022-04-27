#include <WLib_StateMachine.hpp>
#include <ut_catch.hpp>

enum class States_A
{
  A,
  B,
  C,
};

enum class Events_A
{
  a,
  b,
  c,
  d,
};

using dummy_state_machine_types = WLib::StateMachine::state_machine_types<States_A, Events_A>;

class dummy_state: public dummy_state_machine_types::state_interface_t
{
  states_t const m_state;

public:
  dummy_state(states_t const& state) noexcept
    : m_state(state)
  {
  }

  virtual opt_events_t operator()() noexcept override
  {
    if (this->m_state == states_t::A)
      return events_t::a;
    return {};
  }
  virtual opt_events_t handle_event(events_t const& event) noexcept override
  {
    if (event == events_t::a)
      return events_t::c;
    return event;
  }
  virtual states_t get_state() const noexcept override { return this->m_state; }
};

struct dummy_ex_counter
{
  std::size_t constructor_count = 0;
  std::size_t destructor_count  = 0;
  std::size_t call_count        = 0;
  std::size_t handle_count      = 0;
};

class dummy_state_adv: public dummy_state_machine_types::state_interface_t
{
private:
  dummy_ex_counter& m_obj;

public:
  dummy_state_adv(dummy_ex_counter& obj) noexcept
    : m_obj(obj)
  {
    this->m_obj.constructor_count++;
  }
  virtual ~dummy_state_adv() { this->m_obj.destructor_count++; }

  virtual opt_events_t operator()() noexcept override
  {
    this->m_obj.call_count++;
    return {};
  }
  virtual opt_events_t handle_event(events_t const& event) noexcept override
  {
    this->m_obj.handle_count++;
    return event;
  }
  virtual states_t get_state() const noexcept override { return states_t::C; }
};

class dummy_factory: public WLib::StateMachine::Placement_State_Factory<States_A, Events_A, dummy_state, dummy_state_adv>
{
  dummy_ex_counter m_obj_count;

public:
  virtual state_interface_t& construct_state(const states_t& state) noexcept override
  {
    if (state == states_t::C)
      return *new (&this->m_mem) dummy_state_adv(this->m_obj_count);
    return *new (&this->m_mem) dummy_state(state);
  }

  dummy_ex_counter get_obj_count() const { return this->m_obj_count; }
};

TEST_CASE("Test Dummy State")
{
  SECTION("A")
  {
    dummy_state state(States_A::A);
    REQUIRE(state.get_state() == States_A::A);

    {
      auto evt = state();
      REQUIRE(evt.has_value());
      REQUIRE(evt.value() == Events_A::a);
    }

    {
      auto evt = state.handle_event(Events_A::a);
      REQUIRE(evt.has_value());
      REQUIRE(evt.value() == Events_A::c);
    }
    {
      auto evt = state.handle_event(Events_A::b);
      REQUIRE(evt.has_value());
      REQUIRE(evt.value() == Events_A::b);
    }
    {
      auto evt = state.handle_event(Events_A::c);
      REQUIRE(evt.has_value());
      REQUIRE(evt.value() == Events_A::c);
    }
  }
  SECTION("B")
  {
    dummy_state state(States_A::B);
    REQUIRE(state.get_state() == States_A::B);

    REQUIRE(!state().has_value());

    {
      auto evt = state.handle_event(Events_A::a);
      REQUIRE(evt.has_value());
      REQUIRE(evt.value() == Events_A::c);
    }
    {
      auto evt = state.handle_event(Events_A::b);
      REQUIRE(evt.has_value());
      REQUIRE(evt.value() == Events_A::b);
    }
    {
      auto evt = state.handle_event(Events_A::c);
      REQUIRE(evt.has_value());
      REQUIRE(evt.value() == Events_A::c);
    }
  }
  SECTION("C")
  {
    dummy_state state(States_A::C);
    REQUIRE(state.get_state() == States_A::C);

    REQUIRE(!state().has_value());

    {
      auto evt = state.handle_event(Events_A::a);
      REQUIRE(evt.has_value());
      REQUIRE(evt.value() == Events_A::c);
    }
    {
      auto evt = state.handle_event(Events_A::b);
      REQUIRE(evt.has_value());
      REQUIRE(evt.value() == Events_A::b);
    }
    {
      auto evt = state.handle_event(Events_A::c);
      REQUIRE(evt.has_value());
      REQUIRE(evt.value() == Events_A::c);
    }
  }
}

TEST_CASE("Test Dummy Factory")
{
  dummy_factory fac;

  std::initializer_list<States_A> states = {States_A::A, States_A::B, States_A::C};
  for (auto tmp : states)
  {
    dummy_factory::state_interface_t& tmp_state = fac.construct_state(tmp);
    REQUIRE(tmp_state.get_state() == tmp);
    fac.destruct_state(tmp_state);
  }
}

TEST_CASE("Test Engine")
{
  dummy_factory fac;

  constexpr dummy_state_machine_types::transition_t trans_table[] = {
    {States_A::A, Events_A::a, States_A::B},
    {States_A::B, Events_A::b, States_A::C},
    {States_A::C, Events_A::a, States_A::C},
    {Events_A::d, States_A::A},
  };

  {
    dummy_state_machine_types::engine_t eng(trans_table, fac, States_A::A);

    const std::tuple<States_A, dummy_state_machine_types::opt_events_t, dummy_state_machine_types::opt_events_t, States_A, dummy_state_machine_types::opt_events_t, States_A>
      ref_states[] = {
        {States_A::A, {}, {}, States_A::A, {}, States_A::B},
        {States_A::B, {}, {}, States_A::B, {}, States_A::B},
        {States_A::B, Events_A::a, Events_A::c, States_A::B, {}, States_A::B},
        {States_A::B, Events_A::d, {}, States_A::A, {}, States_A::B},
        {States_A::B, Events_A::b, {}, States_A::C, {}, States_A::C},
        {States_A::C, {}, {}, States_A::C, {}, States_A::C},
        {States_A::C, Events_A::d, {}, States_A::A, {}, States_A::B},
        {States_A::B, Events_A::b, {}, States_A::C, {}, States_A::C},
        {States_A::C, {}, {}, States_A::C, {}, States_A::C},
        {States_A::C, {}, {}, States_A::C, {}, States_A::C},
        {States_A::C, {}, {}, States_A::C, {}, States_A::C},
      };

    REQUIRE(eng.get_state() == States_A::A);
    for (auto [init_state, opt_evt, resp_opt_event, state_post_evt, resp_tick, state_post_tick] : ref_states)
    {
      REQUIRE(eng.get_state() == init_state);
      if (opt_evt.has_value())
      {
        auto tmp = eng.handle_event(opt_evt.value());
        REQUIRE(tmp.has_value() == resp_opt_event.has_value());
        if (tmp.has_value() && resp_opt_event.has_value())
        {
          REQUIRE(tmp == resp_opt_event);
        }
      }

      REQUIRE(eng.get_state() == state_post_evt);
      {
        auto tmp = eng();
        REQUIRE(tmp.has_value() == resp_tick.has_value());
        if (tmp.has_value() && resp_tick.has_value())
        {
          REQUIRE(tmp == resp_tick);
        }
      }

      REQUIRE(eng.get_state() == state_post_tick);
    }
  }

  auto counter = fac.get_obj_count();
  REQUIRE(counter.constructor_count == 2);
  REQUIRE(counter.destructor_count == 2);
  REQUIRE(counter.call_count == 6);
  REQUIRE(counter.handle_count == 1);
}