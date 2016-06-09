#pragma once

#include <sstream>
#include <stdexcept>

namespace verilog {
  enum class LogicValue { 
    Zero, True, False, Invalid
  };

  void checkValid(LogicValue v) {
    switch(v) {
      case LogicValue::True:
      case LogicValue::False:
        return;
      default:
        std::stringstream ss;
        ss << "Error: Logic value <>";
        throw std::invalid_argument(ss.str());
    }
  }

  LogicValue operator!(LogicValue v) {
    checkValid(v);
    return (v == LogicValue::True) ? LogicValue::False : LogicValue::True;
  }

  LogicValue fromBool(bool b) {
    return b ? LogicValue::True : LogicValue::False;
  }

  bool toBool(LogicValue v) {
    checkValid(v);
    return v == LogicValue::True;
  }

  LogicValue operator&&(LogicValue v1, LogicValue v2) {
    checkValid(v1);
    checkValid(v2);
    if (v1 == LogicValue::False)
      return v1;
    return v2;
  }

}
