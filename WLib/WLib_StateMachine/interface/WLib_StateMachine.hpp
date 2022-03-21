#pragma once
#ifndef WLIB_STATEMACHINE
#define WLIB_STATEMACHINE

#include <optional>
#include <type_traits>

namespace WLib
{
  namespace StateMachine
  {
    namespace Internal
    {
      template <typename St,
                typename Ev,
                typename = std::enable_if_t<std::is_enum_v<St> && std::is_enum_v<Ev>>>
      struct state_machine_traits
      {
      };
    }    // namespace Internal

    template <typename St, typename Ev> class State_interface;
    template <typename St, typename Ev> class Transition;
    template <typename St, typename Ev> class Factory_interface;
    template <typename St, typename Ev> class Engine;

    template <typename St, typename Ev>
    class State_interface: public Internal::state_machine_traits<St, Ev>
    {
    public:
      using states_t     = St;
      using events_t     = Ev;
      using opt_events_t = std::optional<events_t>;

      virtual ~State_interface() = default;

      virtual opt_events_t operator()() noexcept                  = 0;
      virtual opt_events_t handle_event(events_t const&) noexcept = 0;
      virtual states_t     get_state() const noexcept             = 0;
    };

    template <typename St, typename Ev> class State_base: public State_interface<St, Ev>
    {
    public:
      using states_t     = St;
      using events_t     = Ev;
      using opt_events_t = std::optional<events_t>;

    protected:
      states_t m_state;

    public:
      State_base(states_t const& state)
          : m_state(state)
      {
      }
      virtual ~State_base() = default;

      virtual opt_events_t operator()() noexcept { return std::nullopt; }
      virtual opt_events_t handle_event(events_t const& event) noexcept { return event; }
      virtual states_t     get_state() const noexcept { return this->m_state; }
    };

    template <typename St, typename Ev>
    class Transition: public Internal::state_machine_traits<St, Ev>
    {
      enum class Type
      {
        empty,
        to_target,
        source_to_target,
      };

      Type const m_type   = Type::empty;
      St const   m_source = St();
      Ev const   m_event  = Ev();
      St const   m_target = St();

    public:
      using states_t     = St;
      using events_t     = Ev;
      using opt_states_t = std::optional<states_t>;

      constexpr Transition() noexcept = default;
      constexpr Transition(events_t const& event, states_t const& target) noexcept
          : m_type(Type::to_target)
          , m_event(event)
          , m_target(target)
      {
      }
      constexpr Transition(states_t const& source,
                           events_t const& event,
                           states_t const& target) noexcept
          : m_type(Type::source_to_target)
          , m_source(source)
          , m_event(event)
          , m_target(target){};

      constexpr opt_states_t operator()(states_t const& current_state,
                                        events_t const& event) const noexcept
      {
        if (this->m_type == Type::empty)
          return std::nullopt;

        if (event != this->m_event)
          return std::nullopt;

        if (this->m_type == Type::to_target)
        {
          if (current_state == this->m_target)
            return std::nullopt;
          return this->m_target;
        }

        if (this->m_type == Type::source_to_target)
        {
          if (current_state != this->m_source)
            return std::nullopt;
          return this->m_target;
        }
        return std::nullopt;
      }
    };

    template <typename St, typename Ev>
    class Factory_interface: public Internal::state_machine_traits<St, Ev>
    {
    public:
      using states_t          = St;
      using state_interface_t = State_interface<St, Ev>;

      virtual state_interface_t& construct_state(const states_t&) noexcept   = 0;
      virtual void               destruct_state(state_interface_t&) noexcept = 0;
    };

    template <typename St, typename Ev, typename... T>
    class Placement_State_Factory: public Factory_interface<St, Ev>
    {
    protected:
      std::aligned_union_t<0, T...> m_mem = {};

    public:
      using state_interface_t = State_interface<St, Ev>;
      virtual void destruct_state(state_interface_t& state) noexcept { state.~State_interface(); }
    };

    template <typename St, typename Ev> class Engine: public Internal::state_machine_traits<St, Ev>
    {
    public:
      using states_t            = St;
      using events_t            = Ev;
      using opt_states_t        = std::optional<states_t>;
      using opt_events_t        = std::optional<events_t>;
      using state_interface_t   = State_interface<St, Ev>;
      using transition_t        = Transition<St, Ev>;
      using factory_interface_t = Factory_interface<St, Ev>;

    private:
      transition_t const*  m_tab = nullptr;
      std::size_t          m_len = 0;
      factory_interface_t& m_fac;
      state_interface_t*   m_cur = nullptr;

      void preform_trasition(states_t const& target_state) noexcept
      {
        this->m_fac.destruct_state(*this->m_cur);
        this->m_cur = &this->m_fac.construct_state(target_state);
      }

      opt_states_t search_in_table(states_t const& current_state, events_t const& event)
      {
        for (std::size_t i = 0; i < this->m_len; i++)
        {
          opt_states_t next = this->m_tab[i](current_state, event);
          if (next.has_value())
            return next;
        }
        return std::nullopt;
      }

      opt_events_t handle_opt_event(opt_events_t const& event)
      {
        if (!event.has_value())
          return std::nullopt;

        opt_states_t next = this->search_in_table(this->m_cur->get_state(), event.value());

        if (!next.has_value())
          return event.value();

        this->preform_trasition(next.value());
        return std::nullopt;
      }

    public:
      constexpr Engine(transition_t const*  table,
                       std::size_t const&   table_length,
                       factory_interface_t& state_factory,
                       states_t const&      inital_state) noexcept
          : m_tab(table)
          , m_len(table_length)
          , m_fac(state_factory)
          , m_cur(&this->m_fac.construct_state(inital_state))
      {
      }

      template <std::size_t N>
      constexpr Engine(transition_t const (&tab)[N],
                       factory_interface_t& factory,
                       states_t const&      inital_state) noexcept
          : Engine(tab, N, factory, inital_state)
      {
      }

      virtual ~Engine() { this->m_fac.destruct_state(*this->m_cur); }

      opt_events_t operator()() noexcept
      {
        opt_events_t opt_evt = (*this->m_cur)();
        return handle_opt_event(opt_evt);
      }

      opt_events_t handle_event(events_t const& event)
      {
        opt_events_t opt_evt = this->m_cur->handle_event(event);
        return handle_opt_event(opt_evt);
      }

      states_t get_state() const noexcept { return this->m_cur->get_state(); }
    };

    template <typename St, typename Ev>
    struct state_machine_types: public Internal::state_machine_traits<St, Ev>
    {
      using states_t                                     = St;
      using events_t                                     = Ev;
      using opt_states_t                                 = std::optional<states_t>;
      using opt_events_t                                 = std::optional<events_t>;
      using state_interface_t                            = State_interface<St, Ev>;
      using state_base_t                                 = State_base<St, Ev>;
      using factory_interface_t                          = Factory_interface<St, Ev>;
      template <typename... T> using placement_factory_t = Placement_State_Factory<St, Ev, T...>;
      using engine_t                                     = Engine<St, Ev>;
      using transition_t                                 = Transition<St, Ev>;
    };
  }    // namespace StateMachine
}    // namespace WLib
#endif    // !WLIB_STATEMACHINE
