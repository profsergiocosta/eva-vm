
#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <vector>
#include "EvaValue.h"
#include "../../logger.h"

struct GlobalVar
{
    /* data */
    std::string name;
    EvaValue value;
};


struct Global
{
    /* data */

    
    
    std::vector<GlobalVar> _globals;

    GlobalVar& get(size_t index) { return _globals[index];}

    void set (size_t index, const EvaValue& value) {
        if (index >= _globals.size()) {
            DIE << "Global " << index <<  " doesn't exist.";
        }
        _globals[index].value = value;
    }

    void define (const std::string& name) {
        auto index = getGlobalIndex(name);

        if (index != -1){
            return;
        }
        _globals.push_back({name, NUMBER(0)});
    }

    bool exists (const std::string& name) {
        return getGlobalIndex(name) != -1;

    }



    void addConst (const std::string& name, double value) {
      

        if (exists(name)){
            return;
        }
        
        _globals.push_back({name, NUMBER(value)});

        std::cout <<  name << " " << value << std::endl;
    }

    int getGlobalIndex (const std::string& name) {
        if (_globals.size() > 0) {        
            for (auto i = 0 ; i < _globals.size(); i++) {        
                if (_globals[i].name == name) {
                    return i;
                }
            }
        }
        return -1;
    }
};



#endif