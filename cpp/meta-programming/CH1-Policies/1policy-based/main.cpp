#include <iostream>
#include <string>

class OutputPolicyWriteToCout {
protected:
    template<typename MessageType>
    void Print(MessageType&& message) const {
        std::cout << message << '\n';
    }
};

class LangPolicyEnglish {
protected:
    std::string Message() const { return "Hello world:)"; }
};

class LangPolicyGerman {
protected:
    std::string Message() const { return "Hallo welt:("; }
};

template<typename OutputPolicy, typename LanguagePolicy>
class HelloWorld : private OutputPolicy, private LanguagePolicy {
public:
    void Run() const {
        Print(Message());
    }
private:
    using LanguagePolicy::Message;
    using OutputPolicy::Print;
};

template<bool>
struct CompileTimeError;

template<>
struct CompileTimeError<true> {};

#define STATIC_CHECK(expr) \
    (CompileTimeError<(expr) != 0>())

template<bool>
struct CompileTimeChecker
{
    CompileTimeChecker(...);
};

template<>
struct CompileTimeChecker<true> {};

template<typename To, typename From>
To safe_reintrepret_cast(From from) {
    {
        class ErrorDestination_type_Too_Narrow{};
        (void)sizeof(
                CompileTimeChecker<(sizeof(From) <= sizeof(To))>(
                    ErrorDestination_type_Too_Narrow()));
    }
    return reinterpret_cast<To>(from);
}
int main() {
    using HW_ENG = HelloWorld<OutputPolicyWriteToCout, LangPolicyEnglish>;
    using HW_GER = HelloWorld<OutputPolicyWriteToCout, LangPolicyGerman>;

    HW_ENG eng;
    eng.Run();
    HW_GER ger;
    ger.Run();

    STATIC_CHECK(true);

    void * ptr = nullptr;
    char c = 'a';
    ptr = safe_reintrepret_cast<void*, char>(c);
    //char res = safe_reintrepret_cast<char, void*>(ptr);
    return 0;
}
