#include <WLib_StateMachine.hpp>
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

class process
{
public:
  enum class States
  {
    initialize,
    ready,
    prepare_action,
    action,
    clean_after,
    done,

    aborting,
    aborted,

    error,
  };

private:
  enum class Events
  {
    start,
    next,
    abort,
    complete,
  };

  using state_machine_t = WLib::StateMachine::state_machine_types<States, Events>;
  static constexpr state_machine_t::transition_t trans_table[] = {
    {States::initialize, Events::next, States::ready},
    {States::ready, Events::start, States::prepare_action},

    {States::clean_after, Events::next, States::done},

    {States::prepare_action, Events::abort, States::aborting},
    {States::action, Events::abort, States::aborting},
    {States::clean_after, Events::abort, States::aborting},

    {States::done, Events::complete, States::ready},
    {States::aborted, Events::complete, States::ready},
  };

  class initialize: public state_machine_t::state_base_t
  {
  private:
    std::string& m_str;
    std::size_t  m_count = 0;

  public:
    initialize(std::string& output_str)
      : State_base(States::initialize)
      , m_str(output_str)
    {
      this->m_str.append("enter initialize\n");
    }
    virtual ~initialize() { this->m_str.append("exit initialize\n"); }

    virtual opt_events_t operator()() noexcept
    {
      std::stringstream tmp;
      tmp << "call initialize " << this->m_count << std::endl;
      this->m_str.append(tmp.str());

      this->m_count++;
      if (this->m_count < 10)
        return std::nullopt;
      return Events::next;
    }

    virtual opt_events_t handle_event(events_t const& event) noexcept
    {
      this->m_str.append("initialize handles incomming event\n");
      return event;
    }
  };

  class ready: public state_machine_t::state_base_t
  {
  private:
    std::string& m_str;

  public:
    ready(std::string& output_str)
      : State_base(States::ready)
      , m_str(output_str)
    {
      this->m_str.append("enter ready\n");
    }
    virtual ~ready() { this->m_str.append("exit ready\n"); }
  };

  class process_step: public state_machine_t::state_base_t
  {
  private:
    std::string& m_str;
    std::size_t  m_count = 0;

  public:
    process_step(std::string& output_str)
      : State_base(States::prepare_action)
      , m_str(output_str)
    {
      this->m_str.append("enter process_step\n");
    }
    virtual ~process_step() { this->m_str.append("exit process_step\n"); }

    virtual opt_events_t operator()() noexcept
    {
      std::stringstream tmp;
      tmp << "call process_step " << this->m_count << std::endl;
      this->m_str.append(tmp.str());
      this->m_count++;
      if (this->m_count < 100)
        return std::nullopt;
      return Events::next;
    }

    virtual opt_events_t handle_event(events_t const& event) noexcept
    {
      this->m_str.append("initialize handles incomming event\n");
      return event;
    }

    virtual states_t get_state() const noexcept
    {
      if (this->m_count < 25)
        return states_t::prepare_action;
      if (this->m_count < 75)
        return states_t::action;

      return states_t::clean_after;
    }
  };

  class done: public state_machine_t::state_base_t
  {
  private:
    std::string& m_str;

  public:
    done(std::string& output_str)
      : State_base(States::done)
      , m_str(output_str)
    {
      this->m_str.append("enter done\n");
    }
    virtual ~done() { this->m_str.append("exit done\n"); }
  };

  class aborting: public state_machine_t::state_base_t
  {
  private:
    std::string& m_str;
    std::size_t  m_count = 0;

  public:
    aborting(std::string& output_str)
      : State_base(States::aborting)
      , m_str(output_str)
    {
      this->m_str.append("enter aborting\n");
    }
    virtual ~aborting() { this->m_str.append("exit aborting\n"); }

    virtual opt_events_t operator()() noexcept
    {
      this->m_str.append("call aborting\n");
      this->m_count++;
      if (this->m_count < 100)
        return std::nullopt;
      return Events::next;
    }

    virtual opt_events_t handle_event(events_t const& event) noexcept
    {
      this->m_str.append("abort handles incomming event\n");
      return event;
    }

    virtual states_t get_state() const noexcept
    {
      if (this->m_count < 75)
        return states_t::aborting;

      return states_t::aborted;
    }
  };

  class factory_t: public state_machine_t::placement_factory_t<initialize, ready, process_step, done, aborting>
  {
    std::string& m_str;

  public:
    factory_t(std::string& output_str)
      : m_str(output_str)
    {
    }

    virtual state_interface_t& construct_state(const states_t& state) noexcept
    {
      switch (state)
      {
      case states_t::initialize :
        return *new (&this->m_mem) initialize(this->m_str);

      case states_t::ready :
        return *new (&this->m_mem) ready(this->m_str);

      case states_t::prepare_action :
        return *new (&this->m_mem) process_step(this->m_str);

      case states_t::done :
        return *new (&this->m_mem) done(this->m_str);

      case states_t::aborting :
        return *new (&this->m_mem) aborting(this->m_str);

      default :
        break;
      }
      return *new (&this->m_mem) state_machine_t::state_base_t(states_t::error);
    }
  };

  std::atomic<bool>         m_run = true;
  mutable std::mutex        m_tex;
  std::thread               m_worker;
  factory_t                 m_fac;
  state_machine_t::engine_t m_eng{process::trans_table, m_fac, States::initialize};

  void worker_loop()
  {
    while (this->m_run)
    {
      {
        std::lock_guard<std::mutex> lock(this->m_tex);
        this->m_eng();
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }

public:
  process(std::string& str)
    : m_fac(str)
  {
    this->m_worker = std::thread(
      [&]()
      {
        this->worker_loop();
      });
  }
  ~process()
  {
    this->m_run = false;
    this->m_worker.join();
  }

  void start()
  {
    std::lock_guard<std::mutex> lock(this->m_tex);
    this->m_eng.handle_event(Events::start);
  }
  void complete()
  {
    std::lock_guard<std::mutex> lock(this->m_tex);
    this->m_eng.handle_event(Events::complete);
  }
  void abort()
  {
    std::lock_guard<std::mutex> lock(this->m_tex);
    this->m_eng.handle_event(Events::abort);
  }

  States get_state() const noexcept
  {
    std::lock_guard<std::mutex> lock(this->m_tex);
    return this->m_eng.get_state();
  }
};

int main()
{
  std::string output_string = "";
  auto        job           = [&]()
  {
    std::chrono::steady_clock::time_point deadline;
    process                               pro(output_string);

    deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    while (pro.get_state() == process::States::initialize)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      if (deadline < std::chrono::steady_clock::now())
        return -1;
    }

    if (pro.get_state() != process::States::ready)
      return -2;

    pro.start();

    deadline = std::chrono::steady_clock::now() + std::chrono::seconds(20);
    while (pro.get_state() != process::States::done)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      if (deadline < std::chrono::steady_clock::now())
        return -3;
    }

    pro.complete();

    if (pro.get_state() != process::States::ready)
      return -4;

    pro.start();

    deadline = std::chrono::steady_clock::now() + std::chrono::seconds(20);
    while (pro.get_state() != process::States::done)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      if (deadline < std::chrono::steady_clock::now())
        return -5;
    }

    pro.complete();

    if (pro.get_state() != process::States::ready)
      return -6;

    pro.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    pro.abort();

    deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while (pro.get_state() != process::States::aborted)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      if (deadline < std::chrono::steady_clock::now())
        return -7;
    }

    pro.complete();

    if (pro.get_state() != process::States::ready)
      return -8;
    return 0;
  };
  auto ret = job();
  std::cout << output_string;
  return ret;
}