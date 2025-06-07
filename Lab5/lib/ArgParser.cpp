#include"ArgParser.h"

#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<string_view>

namespace ArgumentParser {

    ArgParseException::ArgParseException(const std::string& msg) {
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


    int getSIndex(std::vector<StringOption>& v, const std::string& lName) {
        for (int i = 0; i < v.size(); i++)
            if (v[i].longName == lName) return i;
        return -1;
    }


    int getIIndex(std::vector<IntOption>& v, const std::string& lName) {
        for (int i = 0; i < v.size(); i++)
            if (v[i].longName == lName) return i;
        return -1;
    }


    int getFIndex(std::vector<FlagOption>& v, const std::string& lName) {
        for (int i = 0; i < v.size(); i++)
            if (v[i].longName == lName) return i;
        return -1;
    }


    int getSIndexShort(std::vector<StringOption>& v, const char sName) {
        for (int i = 0; i < v.size(); i++)
            if (v[i].shortName == sName) return i;
        return -1;
    }


    int getIIndexShort(std::vector<IntOption>& v, const char sName) {
        for (int i = 0; i < v.size(); i++)
            if (v[i].shortName == sName) return i;

        return -1;
    }


    int getFIndexShort(std::vector<FlagOption>& v, const char sName) {
        for (int i = 0; i < v.size(); i++)
            if (v[i].shortName == sName) return i;

        return -1;
    }


    int getPositionalIOptions(std::vector<IntOption>& v) {
        int count = 0;
        for (int i = 0; i < v.size(); i++)
            if (v[i].isPositional) count++;

        return count;
    }


    int getPositionalIOptionIndex(std::vector<IntOption>& v) {
        for (int i = 0; i < v.size(); i++)
            if (v[i].isPositional) return i;

        return -1;
    }



    bool isStrExist(std::vector<std::string>& v, const std::string& str) {
        for (int i = 0; i < v.size(); i++)
            if (v[i] == str) return true;

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
        if (lName.size() < 1) return false;

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
        if (helpOption != nullptr) delete helpOption;
    }

    StringOption& ArgParser::AddStringArgument(const std::string& lName) {

        if (isStrExist(longArgNames, lName)) {

            std::ostringstream out;
            out << "Two options with long name '" << lName << "'";
            throw ArgParseException(out.str());
        }
        sOptions.emplace_back(lName);

        longArgNames.push_back(lName);

        return sOptions[getSIndex(sOptions, lName)];
    }

    StringOption& ArgParser::AddStringArgument(const char sName, const std::string& lName) {

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

        sOptions.emplace_back(sName, lName);

        shortArgNames.push_back(sName);

        longArgNames.push_back(lName);

        return sOptions[getSIndex(sOptions, lName)];
    }

    StringOption& ArgParser::AddStringArgument(const char sName, const std::string& lName, const std::string desc) {

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

        StringOption newOption(sName, lName, desc);
        sOptions.push_back(newOption);

        shortArgNames.push_back(sName);

        longArgNames.push_back(lName);

        return sOptions[getSIndex(sOptions, lName)];
    }


    IntOption& ArgParser::AddIntArgument(const std::string& lName) {

        if (isStrExist(longArgNames, lName)) {
            std::ostringstream out;
            out << "Two options with long name '" << lName << "'";
            throw ArgParseException(out.str());
        }

        iOptions.emplace_back(lName);

        longArgNames.push_back(lName);

        return iOptions[getIIndex(iOptions, lName)];
    }

    IntOption& ArgParser::AddIntArgument(const std::string& lName, const std::string& desc) {

        if (isStrExist(longArgNames, lName)) {
            std::ostringstream out;
            out << "Two options with long name '" << lName << "'";
            throw ArgParseException(out.str());
        }


        iOptions.emplace_back(lName, desc);

        longArgNames.push_back(lName);

        return iOptions[getIIndex(iOptions, lName)];
    }

    IntOption& ArgParser::AddIntArgument(const char sName, const std::string& lName) {

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

        iOptions.emplace_back(sName, lName);;

        shortArgNames.push_back(sName);

        longArgNames.push_back(lName);

        return iOptions[getIIndex(iOptions, lName)];
    }


    FlagOption& ArgParser::AddFlag(const char sName, const std::string& lName) {

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

        fOptions.emplace_back(sName, lName);

        shortArgNames.push_back(sName);

        longArgNames.push_back(lName);

        return fOptions[getFIndex(fOptions, lName)];
    }

    FlagOption& ArgParser::AddFlag(const std::string& lName, const std::string& desc) {

        if (isStrExist(longArgNames, lName)) {
            std::ostringstream out;
            out << "Two options with long name '" << lName << "'";
            throw ArgParseException(out.str());
        }


        fOptions.emplace_back(lName, desc);

        longArgNames.push_back(lName);

        return fOptions[getFIndex(fOptions, lName)];
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

        shortArgNames.push_back(sName);

        longArgNames.push_back(lName);

        return fOptions[getFIndex(fOptions, lName)];
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
        int n = v.size();
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
                    optIndex = getSIndex(sOptions, lName);

                    if (optIndex != -1) {
                        if (!sOptions[optIndex].addValue(value)) return false;

                        argIndex++;
                        continue;
                    }

                    optIndex = getIIndex(iOptions, lName);

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
                optIndex = getSIndexShort(sOptions, sName);

                if (optIndex != -1) {
                    if (!sOptions[optIndex].addValue(value)) return false;

                    argIndex++;
                    continue;
                }

                optIndex = getIIndexShort(iOptions, sName);

                if (optIndex != -1) {
                    if (!parseInt(value, num)) return false;
                    if (!iOptions[optIndex].addValue(num)) return false;

                    argIndex++;
                    continue;
                }

                return false;
            }

            if (isLongOption(curArg, lName)) {
                optIndex = getFIndex(fOptions, lName);

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
                for (int i = 0; i < sFNames.size(); i++) {
                    char sName = sFNames[i];

                    optIndex = getFIndexShort(fOptions, sName);

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


        for (int i = 0; i < sOptions.size(); i++) {
            if (!sOptions[i].isOptionFilled()) return false;
        }

        for (int i = 0; i < iOptions.size(); i++) {
            if (!iOptions[i].isOptionFilled()) return false;
        }
        return true;
    }

    bool ArgParser::Parse(int argc, char* argv[]) {
        std::vector<std::string> v;
        for (int i = 0; i < argc; i++)
            v.push_back(argv[i]);

        return ArgParser::Parse(v);
    }

    const std::string& ArgParser::GetStringValue(const std::string& lName) {
        int index = getSIndex(sOptions, lName);

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
        int index = getIIndex(iOptions, lName);

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
        int index = getIIndex(iOptions, lName);

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
        int index = getFIndex(fOptions, lName);

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

    bool ArgParser::Help() {
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

        if ((helpOption != nullptr) && (helpOption->description.size() > 0))
            out << helpOption->description << std::endl;
        out << std::endl;

        for (int i = 0; i < sOptions.size(); i++)
            out << sOptions[i].getOptionDescription() << std::endl;

        for (int i = 0; i < fOptions.size(); i++)
            out << fOptions[i].getOptionDescription() << std::endl;

        for (int i = 0; i < iOptions.size(); i++)
            out << iOptions[i].getOptionDescription() << std::endl;
        out << std::endl;

        if (helpOption != nullptr)
            out << helpOption->getOptionDescription() << std::endl;

        return out.str();
    }
};
