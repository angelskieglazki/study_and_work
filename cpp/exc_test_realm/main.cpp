#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <functional>
#include <exception>
#include <sstream>
#include <string>
#include <vector>
#include <optional>

#define CHECK_IF(cond, reason, additional)                                                 \
    (static_cast<bool>(cond)                                                               \
         ? void(0)                                                                         \
         : throw check_failed(__FILE__, __LINE__, __PRETTY_FUNCTION__, (sstr() << reason), \
                              (sstr("  bool(" #cond ") == false ") << reason << "\n    ->" << additional)))

class sstr
{
    std::ostringstream output;

public:
    explicit sstr()
        : output()
    {
        //
    }

    explicit sstr(std::string msg)
        : output(msg, std::ios_base::ate)
    {
        //
    }

    operator std::string() const
    {
        return output.str(); //
    }

    template <typename T>
    sstr &operator<<(T arg)
    {
        output << arg;
        return *this;
    }
};

inline std::ostream &operator<<(std::ostream &s, const sstr &val)
{
    return s << std::string(val);
}

class check_failed : public std::exception
{
    const std::string reason_;
    const std::string additional_msg;

public:
    check_failed(const char *file, int line, const char *scope, const std::string &reason, const std::string &fail_msg);

    virtual ~check_failed() = default;

    const std::string &reason() const noexcept
    {
        return reason_;
    }

    const char *what() const noexcept override
    {
        return additional_msg.c_str();
    }
};

check_failed::check_failed(const char *file, int line, const char *scope, const std::string &reason, const std::string &fail_msg)
    : reason_(reason),
      additional_msg(sstr() << file << ":" << line << fail_msg)
{
    std::cout << file << "(" << line << "): " << scope << ": \n"
              << fail_msg << "\n";
};

struct guard
{
    using cb_t = std::function<void(std::optional<std::string> /*reason*/, std::string /*debug*/)>;

    std::exception_ptr &ex;
    std::vector<cb_t> rollback;
    cb_t finalize;
    int count = std::uncaught_exceptions();

    explicit guard(std::exception_ptr &ex_, cb_t finalize_) : ex(ex_), finalize(finalize_)
    {
    }

    ~guard()
    {
        int ec = std::uncaught_exceptions();
        if (count == ec)
        {
            std::cout << "OK cnt=" << ec << "\n";
            return;
        }

        std::cout << "FAIL cnt=" << ec << " expected=" << count << "\n";

        std::optional<std::string> reason;
        std::string msg;
        if (ex)
        {
            try
            {
                std::rethrow_exception(ex);
            }
            catch (check_failed const &err)
            {
                reason = err.reason();
                msg = err.what();
            }
            catch (std::exception &e)
            {
                reason = std::nullopt;
                msg = e.what();
            }
            catch (...)
            {
                reason = std::nullopt;
                msg = "<unknown exception>";
            }
        }
        else
        {
            std::cout << "NO ERROR AT ALL!\n";
            std::abort();
        }

        try
        {
            for (auto &cb : rollback)
            {
                cb(reason, msg);
            }
            finalize(reason, msg);
        }
        catch (...)
        {
            // suppress inner exception
        }
    }

    void With(cb_t &&cb)
    {
        rollback.push_back(std::move(cb));
    }
};

void handler(std::vector<std::string> &inventory)
{
  int retval = -1;

    auto copy = inventory;
    std::exception_ptr ex;
    guard transaction( //
        ex,            //
        [&](std::optional<std::string> reason, std::string msg)
        {
            // Only respond with error
            if (reason)
            {
                std::cout << "Sending error '" << *reason << "\n";
            }
            else
            {
                std::cout << "Sending Unknown error!\n";
            } //
        });
    try
    {
        inventory.push_back("broken sword");

        transaction.With( //
            [&](std::optional<std::string> reason, std::string msg)
            {
                std::cout << "rollback inventory, because of '" << msg << "'\n";
                inventory = copy;
            });

        CHECK_IF(inventory.size() < 5,                           // cond
                 "Error: no room in inventory",                  // Reason
                 "Crash! inventory.size() =" << inventory.size() // msg
        );

        // Container* container;
        // Container rollBackContainer;

        transaction.With( //
            [&](std::optional<std::string> reason, std::string msg)
            {
                std::cout << "rollback container, because of '" << msg << "'\n";
                // if(container != nullptr) {
                //     *container = rollBackContainer;
                // }
            });

        // inventory.at(132) = 5;
        // std::cout << "Crash!\n";

        throw std::runtime_error("test exception");
    }
    catch (...)
    {
        ex = std::current_exception();
        throw;
    }
}

int main()
{
    std::vector<std::string> inventory;
    inventory.push_back("Great Lopata");

    std::cout << "Inventory before:\n";
    for (auto i : inventory)
    {
        std::cout << "    " << i << "\n";
    }

    try
    {
        std::cout << "call it\n";
        handler(inventory);
    }
    catch (const std::exception &e)
    {
        std::cout << "Exception caught: " << e.what() << '\n';
    }

    std::cout << "Inventory after:\n";
    for (auto i : inventory)
    {
        std::cout << "    " << i << "\n";
    }
}