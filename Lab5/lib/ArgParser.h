#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace ArgumentParser {

    class ArgParseException {
    public:
        std::string message;

        ArgParseException(const std::string& msg);
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

        SpecialOption() : Option(){}

        SpecialOption(const SpecialOption& specialOption) : Option(specialOption.shortName, specialOption.longName, specialOption.description),
                                                            defValueSet(specialOption.defValueSet),
                                                            defaultValue(specialOption.defaultValue),
                                                            multiValue(specialOption.multiValue),
                                                            minArgsCount(specialOption.minArgsCount),
                                                            value(specialOption.value),
                                                            values(specialOption.values),
                                                            isOutsideRef(specialOption.isOutsideRef),
                                                            isValSaved(specialOption.isValSaved)
        {
            if (isOutsideRef) {
                valueRef = specialOption.valueRef;
                valueRefs = specialOption.valueRefs;
            } else {
                valueRef = &value;
                valueRefs = &values;
            }
        }


        explicit SpecialOption(const std::string& lName) : SpecialOption('\0', lName, ""){}
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
                return valueRefs->size() >= minArgsCount;
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
        explicit StringOption(const StringOption& so) : SpecialOption<std::string>(so){}
        StringOption() : SpecialOption<std::string>(){}
        explicit StringOption(const std::string& lName) : SpecialOption<std::string>(lName){}
        StringOption(const char sName, const std::string& lName) : SpecialOption<std::string>(sName, lName){}
        StringOption(const char sName, const std::string& lName, const std::string& desc) : SpecialOption<std::string>(sName, lName, desc){}

        std::string getOptionDescription() override;
    };


    class IntOption : public SpecialOption<int>{
    public:
        explicit IntOption(const IntOption& io) : SpecialOption<int>(io){}
        IntOption() : SpecialOption<int>(){}
        explicit IntOption(const std::string& lName) : SpecialOption<int>(lName){}
        IntOption(const char sName, const std::string& lName) : SpecialOption<int>(sName, lName){}
        IntOption(const std::string& lName, const std::string& desc) : SpecialOption<int>(lName, desc){}

        std::string getOptionDescription() override;
    };


    class FlagOption : public SpecialOption<bool>{
    public:
        explicit FlagOption(const FlagOption& fo) : SpecialOption<bool>(fo){}
        FlagOption() : SpecialOption<bool>(){}
        FlagOption(const char sName, const std::string& lName) : SpecialOption<bool>(sName, lName){}
        FlagOption(const std::string& lName, const std::string& desc) : SpecialOption<bool>(lName, desc){}
        FlagOption(const char sName, const std::string& lName, const std::string& desc) : SpecialOption<bool>(sName, lName, desc){}

        std::string getOptionDescription() override;
        bool setTrueValue();
    };


    class ArgParser {
    public:

        std::string parserName;

        std::vector<char> shortArgNames;
        std::vector<std::string> longArgNames;

        std::vector<StringOption> sOptions;
        std::vector<IntOption> iOptions;
        std::vector<FlagOption> fOptions;
        HelpOption* helpOption;


        ArgParser(const std::string& pName);

        ~ArgParser();


        StringOption& AddStringArgument(const std::string& lName);
        StringOption& AddStringArgument(const char sName, const std::string& lName);
        StringOption& AddStringArgument(const char sName, const std::string& lName, const std::string desc);

        IntOption& AddIntArgument(const std::string& lName);
        IntOption& AddIntArgument(const std::string& lName, const std::string& desc);
        IntOption& AddIntArgument(const char sName, const std::string& lName);

        FlagOption& AddFlag(const char sName, const std::string& lName);
        FlagOption& AddFlag(const std::string& lName, const std::string& desc);
        FlagOption& AddFlag(const char sName, const std::string& lName, const std::string& desc);

        HelpOption& AddHelp(const char sName, const std::string& lName, const std::string& desc);


        bool Parse(std::vector<std::string>);
        bool Parse(int argc, char* argv[]);


        const std::string& GetStringValue(const std::string& lName);
        int GetIntValue(const std::string& lName);
        int GetIntValue(const std::string& lName, const int index);
        bool GetFlag(const std::string& lName);

        bool Help();
        std::string HelpDescription();
    };


    int getSIndex(std::vector<StringOption>& v, const std::string& lName);
    int getIIndex(std::vector<IntOption>& v, const std::string& lName);
    int getFIndex(std::vector<FlagOption>& v, const std::string& lName);


    int getSIndexShort(std::vector<StringOption>& v, const char sName);


    int getIIndexShort(std::vector<IntOption>& v, const char sName);


    int getFIndexShort(std::vector<FlagOption>& v, const char sName);


    int getPositionalIOptions(std::vector<IntOption>& v);


    int getPositionalIOptionIndex(std::vector<IntOption>& v);


    bool isShortOptionVal(std::string_view str, char& sName, std::string& value);


    bool isLongOptionVal(std::string_view str, std::string& lName,
                         std::string& value);


    bool isLongOption(std::string_view str, std::string& lName);


    bool isShortFlagsOption(std::string_view str, std::string& sFNames);


    bool isStrExist(std::vector<std::string>& v, const std::string& str);


    int getChIndex(std::vector<char>& v, const char ch);


    bool isOption(std::string_view str);


    bool parseInt(const std::string& str, int& num);

}
