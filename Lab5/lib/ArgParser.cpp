#include"ArgParser.h"

#include<iostream>
#include<string>
#include<vector>
#include<string_view>

namespace ArgumentParser {

    ArgParseException::ArgParseException(const std::string& msg){
        message = msg;
    }

    Option::Option(const char sName, const std::string& lName, const std::string& desc) :
        shortName(sName),
        longName(lName),
        description(desc) {}

    Option::Option() :
            shortName('\0'),
            longName(""),
            description("") {}

    std::string IntOption::getOptionDescription(){
        std::ostringstream out;

        if (shortName == '\0') out << "    ";
        else out << "-" << shortName << ",  ";

        out << "--" << longName << "=<int>,  " << description;

        if (multiValue) {
            out << "[repeated, min args = " << minArgsCount << "]";
        }

        return out.str();
    }

    std::string StringOption::getOptionDescription() {
        std::ostringstream out;
        if (shortName == '\0') out << "   ";
        else out << "-" << shortName << ",  ";
        out << "--" << longName << "=<string>,  " << description;
        if (multiValue) {
        out << " [repeated, min args = " << minArgsCount << "]";
    }

        return out.str();
    }

    std::string FlagOption::getOptionDescription() {
        std::ostringstream out;
        out << "-" << shortName << ",  --" << longName << ",  " << description;
        if (defValueSet) {
            out << " [default = " << (defaultValue ? "true" : "false") << "]";
        }

        return out.str();
    }

    std::string HelpOption::getOptionDescription() {
        std::ostringstream out;
        if (shortName != '\0') out << "-" << shortName << ", ";
        out << "--" << longName << " " << description;

        return out.str();
    }


    bool FlagOption::setTrueValue() {
        *valueRef = true;
        isValSaved = true;

        return true;
    }


    bool isOption(std::string_view str) {
        if (str.size() < 2)
            return false;
        if (str[0] != '-')
            return false;

        return true;
    }


    bool parseInt(const std::string& str, int& num) {

        std::istringstream in(str);

        in >> num;

        if (in.fail())
            return false;

        if (!in.eof())
            return false;

        return true;
    }


    int getPositionalIOptions(std::vector<IntOption>& v) {
        int count = 0;
        for (auto & i : v)
            if (i.isPositional) count++;

        return count;
    }


    int getPositionalIOptionIndex(std::vector<IntOption>& v) {
        for (int i = 0; i < v.size(); i++)
            if (v[i].isPositional) return i;

        return -1;
    }


    bool isStrExist(std::vector<std::string>& v, const std::string& str) {
        for (const auto & i : v)
            if (i == str) return true;

        return false;
    }


    int getChIndex(std::vector<char>& v, const char ch) {
        for (int i = 0; i < v.size(); i++)
            if (v[i] == ch) return i;

        return -1;
    }


    bool isShortOptionVal(std::string_view str, char& sName, std::string& value) {

        if (!isOption(str)) return false;

        if (str[1] == '-') return false;

        size_t eqPos = str.find('=');

        if (eqPos == std::string::npos) return false;

        std::string_view sNameStr = str.substr(1, eqPos - 1);

        if (sNameStr.size() != 1) return false;

        sName = sNameStr[0];

        value = str.substr(eqPos + 1);

        return true;
    }


    bool isLongOptionVal(std::string_view str, std::string& lName, std::string& value) {

        if (!isOption(str)) return false;

        if (str[1] != '-') return false;

        size_t eqPos = str.find('=');
        if (eqPos == std::string::npos) return false;

        lName = str.substr(2, eqPos - 2);
        if (lName.empty()) return false;

        value = str.substr(eqPos + 1);

        return true;
    }


    bool isLongOption(std::string_view str, std::string& lName) {

        if (!isOption(str)) return false;

        if (str[1] != '-') return false;
        lName = str.substr(2);

        return true;
    }


    bool isShortFlagsOption(std::string_view str, std::string& sFNames) {

        if (!isOption(str)) return false;
        sFNames = str.substr(1);
        return true;
    }


    ArgParser::ArgParser(const std::string& pName) {
        parserName = pName;
        helpOption = nullptr;
    }

    ArgParser::~ArgParser() {
        delete helpOption;
    }


    StringOption& ArgParser::AddStringArgument(const std::string& lName) {
        return AddStringArgument('\0', lName, "");
    }

    StringOption& ArgParser::AddStringArgument(const char sName, const std::string& lName) {
        return AddStringArgument(sName, lName, "");
    }

    StringOption& ArgParser::AddStringArgument(const char sName, const std::string& lName, const std::string& desc) {
        if (sName != '\0' && getChIndex(shortArgNames, sName) != -1) {
            std::ostringstream out;
            out << "Two options with short name '" << sName << "'";
            throw ArgParseException(out.str());
        }

        if (isStrExist(longArgNames, lName)) {
            std::ostringstream out;
            out << "Two options with long name '" << lName << "'";
            throw ArgParseException(out.str());
        }

        sOptions.emplace_back(sName, lName, desc);

        if (sName != '\0') shortArgNames.push_back(sName);
        longArgNames.push_back(lName);

        return sOptions[getIndex(sOptions, lName)];
    }


    IntOption& ArgParser::AddIntArgument(const std::string& lName) {
        return AddIntArgument('\0', lName, "");
    }

    IntOption& ArgParser::AddIntArgument(const std::string& lName, const std::string& desc) {
        return AddIntArgument('\0', lName, desc);
    }

    IntOption& ArgParser::AddIntArgument(const char sName, const std::string& lName) {
        return AddIntArgument(sName, lName, "");
    }

    IntOption& ArgParser::AddIntArgument(const char sName, const std::string& lName, const std::string& desc) {
        if (sName != '\0' && getChIndex(shortArgNames, sName) != -1) {
            std::ostringstream out;
            out << "Two options with short name '" << sName << "'";
            throw ArgParseException(out.str());
        }

        if (isStrExist(longArgNames, lName)) {
            std::ostringstream out;
            out << "Two options with long name '" << lName << "'";
            throw ArgParseException(out.str());
        }

        iOptions.emplace_back(sName, lName, desc);

        if (sName != '\0') shortArgNames.push_back(sName);
        longArgNames.push_back(lName);

        return iOptions[getIndex(iOptions, lName)];
    }


    FlagOption& ArgParser::AddFlag(const char sName, const std::string& lName) {
        return AddFlag(sName, lName, "");
    }

    FlagOption& ArgParser::AddFlag(const std::string& lName, const std::string& desc) {
        return AddFlag('\0', lName, desc);
    }

    FlagOption& ArgParser::AddFlag(const char sName, const std::string& lName, const std::string& desc) {

        if (getChIndex(shortArgNames, sName) != -1) {

            std::ostringstream out;
            out << "Two options with short name '" << sName << "'";
            throw ArgParseException(out.str());
        }

        if (isStrExist(longArgNames, lName)) {

            std::ostringstream out;
            out << "Two options with long name '" << lName << "'";
            throw ArgParseException(out.str());
        }

        fOptions.emplace_back(sName, lName, desc);

        if (sName != '\0') shortArgNames.push_back(sName);
        longArgNames.push_back(lName);

        return fOptions[getIndex(fOptions, lName)];
    }


    HelpOption& ArgParser::AddHelp(const char sName, const std::string& lName, const std::string& desc) {

        if (getChIndex(shortArgNames, sName) != -1) {
            std::ostringstream out;
            out << "Two options with short name '" << sName << "'";
            throw ArgParseException(out.str());
        }

        if (isStrExist(longArgNames, lName)) {
            std::ostringstream out;
            out << "Two options with long name '" << lName << "'";
            throw ArgParseException(out.str());
        }

        if (helpOption != nullptr) {
            std::ostringstream out;
            out << "Two help options";
            throw ArgParseException(out.str());
        }

        helpOption = new HelpOption(sName, lName, desc);

        shortArgNames.push_back(sName);
        longArgNames.push_back(lName);

        return *helpOption;
    }


    bool ArgParser::Parse(std::vector<std::string> v) {
        size_t n = v.size();
        if (n < 1) return false;

        if (getPositionalIOptions(iOptions) > 1) {
            throw ArgParseException("At least two integer option are positional");
        }

        int posIndex = getPositionalIOptionIndex(iOptions);

        int argIndex = 1;
        int num;
        int optIndex;
        std::string lName;
        char sName;
        std::string sFNames;
        std::string value;

        while (argIndex < n) {
            std::string& curArg = v[argIndex];
            if ((posIndex != -1) && parseInt(curArg, num)) {
                if (!(iOptions[posIndex].addValue(num))) return false;

                argIndex++;
                continue;
            }

            if (!parseInt(curArg, num)) {
                if (isLongOptionVal(curArg, lName, value)) {
                    optIndex = getIndex(sOptions, lName);

                    if (optIndex != -1) {
                        if (!sOptions[optIndex].addValue(value)) return false;

                        argIndex++;
                        continue;
                    }

                    optIndex = getIndex(iOptions, lName);

                    if (optIndex != -1) {
                        if (!parseInt(value, num)) return false;
                        if (!iOptions[optIndex].addValue(num)) return false;

                        argIndex++;
                        continue;
                    }

                    return false;
                }
            }
            if (isShortOptionVal(curArg, sName, value)) {
                optIndex = getIndexShort(sOptions, sName);

                if (optIndex != -1) {
                    if (!sOptions[optIndex].addValue(value)) return false;

                    argIndex++;
                    continue;
                }

                optIndex = getIndexShort(iOptions, sName);

                if (optIndex != -1) {
                    if (!parseInt(value, num)) return false;
                    if (!iOptions[optIndex].addValue(num)) return false;

                    argIndex++;
                    continue;
                }

                return false;
            }

            if (isLongOption(curArg, lName)) {
                optIndex = getIndex(fOptions, lName);

                if (optIndex != -1) {
                    if (!fOptions[optIndex].setTrueValue()) return false;

                    argIndex++;
                    continue;
                }

                if ((helpOption != nullptr) && (helpOption->longName == lName)) {
                    helpOption->isSpecified = true;
                    argIndex++;
                    continue;
                }

                return false;
            }

            if (isShortFlagsOption(curArg, sFNames)) {
                for (char sName : sFNames) {
                    optIndex = getIndexShort(fOptions, sName);

                    if (optIndex != -1) {
                        if (!fOptions[optIndex].setTrueValue()) return false;

                        continue;
                    }

                    if ((helpOption != nullptr) && (helpOption->shortName == sName)) {
                        helpOption->isSpecified = true;
                        continue;
                    }

                    return false;
                }

                argIndex++;
                continue;
            }

            return false;
        }


        for (auto& sOption : sOptions) {
            if (!sOption.isOptionFilled()) return false;
        }

        for (auto& iOption : iOptions) {
            if (!iOption.isOptionFilled()) return false;
        }
        return true;
    }


    bool ArgParser::Parse(int argc, char* argv[]) {
        std::vector<std::string> v;
        for (int i = 0; i < argc; i++)
            v.emplace_back(argv[i]);

        return ArgParser::Parse(v);
    }


    const std::string& ArgParser::GetStringValue(const std::string& lName) {
        int index = getIndex(sOptions, lName);

        if (index == -1) {
            std::ostringstream out;
            out << "No string option with long name '" << lName << "'";
            throw ArgParseException(out.str());
        }

        StringOption& option = sOptions[index];

        if (option.multiValue) {
            std::ostringstream out;
            out << "String option with long name '" << lName <<"' not in multiValue regime. Can't GetIntValue without index";
            throw ArgParseException(out.str());
        }

        if (!option.isValSaved) {
            if (option.defValueSet){
                return option.defaultValue;
            }
            std::ostringstream out;
            out << "String option with long name '" << lName <<"' has no value and has no default value";
            throw ArgParseException(out.str());
        }

        return *option.valueRef;
    }


    int ArgParser::GetIntValue(const std::string& lName) {
        int index = getIndex(iOptions, lName);

        if (index == -1) {
            std::ostringstream out;
            out << "No integer option with long name '" << lName << "'";
            throw ArgParseException(out.str());
        }

        IntOption& option = iOptions[index];

        if (option.multiValue) {
            std::ostringstream out;
            out << "Integer option with long name '" << lName <<
                "' in multiValue regime. Can't GetIntValue without index";
            throw ArgParseException(out.str());
        }

        if (!option.isValSaved) {
            std::ostringstream out;
            out << "Integer option with long name '" << lName <<
                "' has no value";
            throw ArgParseException(out.str());
        }

        return option.value;
    }


    int ArgParser::GetIntValue(const std::string& lName, const int vIndex) {
        int index = getIndex(iOptions, lName);

        if (index == -1) {
            std::ostringstream out;
            out << "No integer option with long name '" << lName << "'";
            throw ArgParseException(out.str());
        }

        IntOption& option = iOptions[index];
        if ((vIndex == 0) && (!option.multiValue)) {
            if (option.isValSaved) {
                return *option.valueRef;
            } else {
                std::ostringstream out;
                out << "Integer option with long name '" << lName <<"' has no value";
                throw ArgParseException(out.str());
            }
        }

        if (!option.multiValue) {
            std::ostringstream out;
            out << "Integer option with long name '" << lName <<
                "' not in multiValue regime. Can't GetIntValue by nonzero index";
            throw ArgParseException(out.str());
        }

        if (option.values.size() < vIndex) {
            std::ostringstream out;
            out << "Integer option with long name '" << lName <<
                "' in multiValue regime has no value with index " << vIndex;
            throw ArgParseException(out.str());
        }

        return (*option.valueRefs)[vIndex];
    }

    bool ArgParser::GetFlag(const std::string& lName) {
        int index = getIndex(fOptions, lName);

        if (index == -1) {
            std::ostringstream out;
            out << "No flag option with long name '" << lName << "'";
            throw ArgParseException(out.str());
        }

        FlagOption& option = fOptions[index];


        if (!option.defValueSet && !option.isValSaved) {
            std::ostringstream out;
            out << "Flag option with long name '" << lName <<"' has no value and has no default value";
            throw ArgParseException(out.str());
        }

        if (option.defValueSet && !option.isValSaved) {

            return option.defaultValue;
        }

        return option.valueRef;
    }

    bool ArgParser::Help() const {
        if (helpOption == nullptr) {
            std::ostringstream out;
            out << "Parser has no help option";
            throw ArgParseException(out.str());
        }

        return helpOption->isSpecified;
    }

    std::string ArgParser::HelpDescription() {
        std::ostringstream out;

        out << parserName << std::endl;

        if ((helpOption != nullptr) && (!helpOption->description.empty()))
            out << helpOption->description << std::endl;
        out << std::endl;

        for (auto& sOption : sOptions)
            out << sOption.getOptionDescription() << std::endl;

        for (auto& fOption : fOptions)
            out << fOption.getOptionDescription() << std::endl;

        for (auto& iOption : iOptions)
            out << iOption.getOptionDescription() << std::endl;
        out << std::endl;

        if (helpOption != nullptr)
            out << helpOption->getOptionDescription() << std::endl;

        return out.str();
    }
}
