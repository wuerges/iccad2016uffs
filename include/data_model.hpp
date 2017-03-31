#pragma once

#include <sstream>
#include <iostream>
#include <stdexcept>

namespace verilog {
  enum class NegP { 
    Positive, Negative   
  };

  enum class LogicValue { 
    Zero, One, Undefined, Invalid
  };

  std::ostream & operator<<(std::ostream& o, LogicValue v) {
    switch(v) {
      case LogicValue::Zero:
        o << "LV::Zero";
        break;
      case LogicValue::One:
        o << "LV::One";
        break;
      case LogicValue::Undefined:
        o << "LV::Undefined";
        break;
      case LogicValue::Invalid:
        o << "LV::Invalid";
        break;
    }
    return o;
  }

  void checkValid(LogicValue v) {
    switch(v) {
      case LogicValue::One:
        return;
      case LogicValue::Zero:
        return;
      default:
        std::stringstream ss;
        ss << "Error: Logic value <" << v << ">";
        throw std::invalid_argument(ss.str());
    }
  }

  LogicValue operator!(LogicValue v) {
    checkValid(v);
    return (v == LogicValue::One) ? LogicValue::Zero : LogicValue::One;
  }

  LogicValue fromBool(bool b) {
    return b ? LogicValue::One : LogicValue::Zero;
  }

  bool toBool(LogicValue v) {
    checkValid(v);
    return v == LogicValue::One;
  }

  LogicValue operator&&(LogicValue v1, LogicValue v2) {
    checkValid(v1);
    checkValid(v2);
    if (v1 == LogicValue::Zero)
      return v1;
    return v2;
  }

}
