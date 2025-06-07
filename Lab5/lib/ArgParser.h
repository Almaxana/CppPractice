#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace ArgumentParser {

    int getChIndex(std::vector<char>& v, char ch);
    bool isStrExist(std::vector<std::string>& v, const std::string& str);

    template <typename OptionContainer>
    int getIndex(const OptionContainer& v, const std::string& lName) {
        for (int i = 0; i < v.size(); i++)
            if (v[i].longName == lName) return i;
        return -1;
    }

    template <typename OptionContainer>
    int getIndexShort(const OptionContainer& v, const char sName) {
        for (int i = 0; i < v.size(); i++)
            if (v[i].shortName == sName) return i;
        return -1;
    }

    class ArgParseException : public std::exception {
    public:
        std::string message;

        explicit ArgParseException(const std::string& msg);
    };


    class Option {
    public:
        char shortName;
        std::string longName;
        std::string description;

        Option();
        Option(char sName, const std::string& lName, const std::string& desc);

        virtual ~Option() = default;

        virtual std::string getOptionDescription() = 0;

    };


    class HelpOption : public Option {
    public:
        bool isSpecified;
        HelpOption(const char sName, const std::string& lName, const std::string& desc) : Option(sName, lName, desc),
                                                                                            isSpecified(false){}

        std::string getOptionDescription() override;
    };


    template <typename T>
    class SpecialOption : public Option {
    public:
        T defaultValue;
        bool defValueSet;
        bool multiValue;
        int minArgsCount;
        T value;
        std::vector<T> values;
        T* valueRef;
        std::vector<T>* valueRefs;
        bool isOutsideRef;
        bool isValSaved;
        bool isPositional;

        SpecialOption() : Option(),
                          defValueSet(false),
                          multiValue(false),
                          minArgsCount(-1),
                          valueRef(&value),
                          isOutsideRef(false),
                          isValSaved(false),
                          isPositional(false) {}

        SpecialOption(const SpecialOption& specialOption) : Option(specialOption.shortName, specialOption.longName, specialOption.description),
                                                            defValueSet(specialOption.defValueSet),
                                                            defaultValue(specialOption.defaultValue),
                                                            multiValue(specialOption.multiValue),
                                                            minArgsCount(specialOption.minArgsCount),
                                                            value(specialOption.value),
                                                            values(specialOption.values),
                                                            isOutsideRef(specialOption.isOutsideRef),
                                                            isValSaved(specialOption.isValSaved),
                                                            isPositional(false)
        {
            if (isOutsideRef) {
                valueRef = specialOption.valueRef;
                valueRefs = specialOption.valueRefs;
            } else {
                valueRef = &value;
                valueRefs = &values;
            }
        }


        explicit SpecialOption(const std::string& lName) : SpecialOption('\0', lName, "") {}
        SpecialOption(const char sName, const std::string& lName) : SpecialOption(sName, lName, ""){}
        SpecialOption(const std::string& lName, const std::string& desc) : SpecialOption('\0', lName, desc){}
        SpecialOption(const char sName, const std::string& lName, const std::string& desc) : Option(sName, lName, desc),
                                                                                             defValueSet(false),
                                                                                             multiValue(false),
                                                                                             minArgsCount(-1),
                                                                                             valueRef(&value),
                                                                                             isOutsideRef(false),
                                                                                             isValSaved(false),
                                                                                             isPositional(false) {}

        bool isOptionFilled() {
            if (multiValue) {
                if (valueRefs != nullptr) return valueRefs->size() >= minArgsCount;
                return values.size() >= minArgsCount;
            } else {
                if (isValSaved) return true;
                if (defValueSet) return true;

                return false;
            }
        }

        SpecialOption& StoreValue(T& rVal) {
            if (multiValue) {
                throw ArgParseException("Option in multiValue regime cannot StoreValue");
            }

            valueRef = &rVal;
            isOutsideRef = true;

            return *this;
        }

        SpecialOption& StoreValues(std::vector<T>& rVals) {

            if (!multiValue) {
                throw ArgParseException("Option not in multiValue regime cannot StoreValues");
            }

            valueRefs = &rVals;
            isOutsideRef = true;

            return *this;
        }

        SpecialOption& MultiValue() {

            if (multiValue) return *this;
            multiValue = true;
            minArgsCount = 0;
            isOutsideRef = false;

            return *this;
        }

        SpecialOption& MultiValue(const int mArgsCount) {
            minArgsCount = mArgsCount;

            if (multiValue) return *this;
            multiValue = true;
            isOutsideRef = false;

            return *this;
        }

        bool addValue(T val) {
            if (multiValue) {
                valueRefs->push_back(val);
            } else {
                if (!isValSaved) {
                    *valueRef = val;
                    isValSaved = true;
                } else {
                    return false;
                }
            }

            return true;
        }

        SpecialOption& Default(const T& dVal) {
            defaultValue = dVal;
            defValueSet = true;

            return *this;
        }

        SpecialOption& Positional() {
            isPositional = true;
            return *this;
        }
    };


    class StringOption : public SpecialOption<std::string>{
    public:
        StringOption(const StringOption& so) : SpecialOption<std::string>(so){}
        StringOption() : SpecialOption<std::string>(){}
        explicit StringOption(const std::string& lName) : SpecialOption<std::string>(lName){}
        StringOption(const char sName, const std::string& lName) : SpecialOption<std::string>(sName, lName){}
        StringOption(const char sName, const std::string& lName, const std::string& desc) : SpecialOption<std::string>(sName, lName, desc){}

        std::string getOptionDescription() override;
    };


    class IntOption : public SpecialOption<int>{
    public:
        IntOption(const IntOption& io) : SpecialOption<int>(io){}
        IntOption() : SpecialOption<int>(){}
        explicit IntOption(const std::string& lName) : SpecialOption<int>(lName){}
        IntOption(const char sName, const std::string& lName) : SpecialOption<int>(sName, lName){}
        IntOption(const std::string& lName, const std::string& desc) : SpecialOption<int>(lName, desc){}
        IntOption(const char sName, const std::string& lName, const std::string& desc) : SpecialOption<int>(sName, lName, desc){}

        std::string getOptionDescription() override;
    };


    class FlagOption : public SpecialOption<bool>{
    public:
        FlagOption(const FlagOption& fo) : SpecialOption<bool>(fo){}
        FlagOption() : SpecialOption<bool>(){}
        FlagOption(const char sName, const std::string& lName) : SpecialOption<bool>(sName, lName){}
        FlagOption(const std::string& lName, const std::string& desc) : SpecialOption<bool>(lName, desc){}
        FlagOption(const char sName, const std::string& lName, const std::string& desc) : SpecialOption<bool>(sName, lName, desc){}

        std::string getOptionDescription() override;
        bool setTrueValue();
    };


    class ArgParser {
    private:

        std::string parserName;

        std::vector<char> shortArgNames;
        std::vector<std::string> longArgNames;

        std::vector<StringOption> sOptions;
        std::vector<IntOption> iOptions;
        std::vector<FlagOption> fOptions;
        HelpOption* helpOption;

    public:
        explicit ArgParser(const std::string& pName);

        ~ArgParser();


        StringOption& AddStringArgument(const std::string& lName);
        StringOption& AddStringArgument(char sName, const std::string& lName);
        StringOption& AddStringArgument(char sName, const std::string& lName, const std::string& desc);

        IntOption& AddIntArgument(const std::string& lName);
        IntOption& AddIntArgument(const std::string& lName, const std::string& desc);
        IntOption& AddIntArgument(char sName, const std::string& lName);
        IntOption& AddIntArgument(char sName, const std::string& lName, const std::string& desc);

        FlagOption& AddFlag(char sName, const std::string& lName);
        FlagOption& AddFlag(const std::string& lName, const std::string& desc);
        FlagOption& AddFlag(char sName, const std::string& lName, const std::string& desc);

        HelpOption& AddHelp(char sName, const std::string& lName, const std::string& desc);


        bool Parse(std::vector<std::string>);
        bool Parse(int argc, char* argv[]);


        const std::string& GetStringValue(const std::string& lName);
        int GetIntValue(const std::string& lName);
        int GetIntValue(const std::string& lName, int index);
        bool GetFlag(const std::string& lName);

        bool Help() const;
        std::string HelpDescription();
    };

    int getPositionalIOptions(std::vector<IntOption>& v);

    int getPositionalIOptionIndex(std::vector<IntOption>& v);

    bool isShortOptionVal(std::string_view str, char& sName, std::string& value);

    bool isLongOptionVal(std::string_view str, std::string& lName,
                         std::string& value);

    bool isLongOption(std::string_view str, std::string& lName);

    bool isShortFlagsOption(std::string_view str, std::string& sFNames);

    bool isOption(std::string_view str);

    bool parseInt(const std::string& str, int& num);
}
