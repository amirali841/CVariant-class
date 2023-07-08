#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
using namespace std;

bool isNumeric(const std::string& str) {
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

class CVariant {
private:
    enum class Type {
        INT8,
        UINT8,
        INT16,
        UINT16,
        INT32,
        UINT32,
        INT64,
        UINT64,
        FLOAT,
        DOUBLE,
        STRING,
        WIDE_STRING,
        ARRAY,
        MAP,
        BYTE_ARRAY
    };

    Type m_type;
    std::vector<CVariant> m_array;
    std::map<std::string, CVariant> m_map;
    std::vector<unsigned char> m_byteArray;

    union {
        int8_t int8Val;
        uint8_t uint8Val;
        int16_t int16Val;
        uint16_t uint16Val;
        int32_t int32Val;
        uint32_t uint32Val;
        int64_t int64Val;
        uint64_t uint64Val;
        float floatVal;
        double doubleVal;
        std::string* stringVal;
        std::wstring* wideStringVal;
    };

public:
    friend ostream& operator<<(std::ostream& os, const CVariant& variant);
    CVariant() : m_type(Type::INT32), int32Val(0) {}

    // Constructors for built-in types
    CVariant(int8_t value) : m_type(Type::INT8), int8Val(value) {}
    CVariant(uint8_t value) : m_type(Type::UINT8), uint8Val(value) {}
    CVariant(int16_t value) : m_type(Type::INT16), int16Val(value) {}
    CVariant(uint16_t value) : m_type(Type::UINT16), uint16Val(value) {}
    CVariant(int32_t value) : m_type(Type::INT32), int32Val(value) {}
    CVariant(uint32_t value) : m_type(Type::UINT32), uint32Val(value) {}
    CVariant(int64_t value) : m_type(Type::INT64), int64Val(value) {}
    CVariant(uint64_t value) : m_type(Type::UINT64), uint64Val(value) {}
    CVariant(float value) : m_type(Type::FLOAT), floatVal(value) {}
    CVariant(double value) : m_type(Type::DOUBLE), doubleVal(value) {}

    // Constructors for string types
    CVariant(const std::string& value) : m_type(Type::STRING), stringVal(new std::string(value)) {}
    CVariant(const std::wstring& value) : m_type(Type::WIDE_STRING), wideStringVal(new std::wstring(value)) {}
    CVariant(const char* value) : m_type(Type::STRING), stringVal(new std::string(value)) {}

    // Constructor for array type
    CVariant(const std::vector<CVariant>& value) : m_type(Type::ARRAY), m_array(value) {}

    // Constructor for map type
    CVariant(const std::map<std::string, CVariant>& value) : m_type(Type::MAP), m_map(value) {}

    // Constructor for byte array type
    CVariant(const std::vector<unsigned char>& value) : m_type(Type::BYTE_ARRAY), m_byteArray(value) {}

    // Copy constructor
    CVariant(const CVariant& other) : m_type(other.m_type) {
        switch (m_type) {
            case Type::INT8:
                int8Val = other.int8Val;
                break;
            case Type::UINT8:
                uint8Val = other.uint8Val;
                break;
            case Type::INT16:
                int16Val = other.int16Val;
                break;
            case Type::UINT16:
                uint16Val = other.uint16Val;
                break;
            case Type::INT32:
                int32Val = other.int32Val;
                break;
            case Type::UINT32:
                uint32Val = other.uint32Val;
                break;
            case Type::INT64:
                int64Val = other.int64Val;
                break;
            case Type::UINT64:
                uint64Val = other.uint64Val;
                break;
            case Type::FLOAT:
                floatVal = other.floatVal;
                break;
            case Type::DOUBLE:
                doubleVal = other.doubleVal;
                break;
            case Type::STRING:
                stringVal = new std::string(*other.stringVal);
                break;
            case Type::WIDE_STRING:
                wideStringVal = new std::wstring(*other.wideStringVal);
                break;
            case Type::ARRAY:
                m_array = other.m_array;
                break;
            case Type::MAP:
                m_map = other.m_map;
                break;
            case Type::BYTE_ARRAY:
                m_byteArray = other.m_byteArray;
                break;
        }
    }

    // Destructor
    ~CVariant() {
        if (m_type == Type::STRING)
            delete stringVal;
        else if (m_type == Type::WIDE_STRING)
            delete wideStringVal;
    }

    // Assignment operator
    CVariant& operator=(const CVariant& other) {
        if (this != &other) {
            // Clean up existing resources
            if (m_type == Type::STRING)
                delete stringVal;
            else if (m_type == Type::WIDE_STRING)
                delete wideStringVal;

            m_type = other.m_type;
            switch (m_type) {
                case Type::INT8:
                    int8Val = other.int8Val;
                    break;
                case Type::UINT8:
                    uint8Val = other.uint8Val;
                    break;
                case Type::INT16:
                    int16Val = other.int16Val;
                    break;
                case Type::UINT16:
                    uint16Val = other.uint16Val;
                    break;
                case Type::INT32:
                    int32Val = other.int32Val;
                    break;
                case Type::UINT32:
                    uint32Val = other.uint32Val;
                    break;
                case Type::INT64:
                    int64Val = other.int64Val;
                    break;
                case Type::UINT64:
                    uint64Val = other.uint64Val;
                    break;
                case Type::FLOAT:
                    floatVal = other.floatVal;
                    break;
                case Type::DOUBLE:
                    doubleVal = other.doubleVal;
                    break;
                case Type::STRING:
                    stringVal = new std::string(*other.stringVal);
                    break;
                case Type::WIDE_STRING:
                    wideStringVal = new std::wstring(*other.wideStringVal);
                    break;
                case Type::ARRAY:
                    m_array = other.m_array;
                    break;
                case Type::MAP:
                    m_map = other.m_map;
                    break;
                case Type::BYTE_ARRAY:
                    m_byteArray = other.m_byteArray;
                    break;
            }
        }
        return *this;
    }

    // Functions to cast to the requested types
    int8_t toInt8() const {
        if (m_type == Type::INT8)
            return int8Val;
        throw std::runtime_error("Cannot cast CVariant to int8_t");
    }

    uint8_t toUInt8() const {
        if (m_type == Type::UINT8)
            return uint8Val;
        throw std::runtime_error("Cannot cast CVariant to uint8_t");
    }

    int16_t toInt16() const {
        if (m_type == Type::INT16)
            return int16Val;
        throw std::runtime_error("Cannot cast CVariant to int16_t");
    }

    uint16_t toUInt16() const {
        if (m_type == Type::UINT16)
            return uint16Val;
        throw std::runtime_error("Cannot cast CVariant to uint16_t");
    }

    int32_t toInt32() const {
        if (m_type == Type::INT32)
            return int32Val;
        if (m_type == Type::STRING && isNumeric(*stringVal))
            return stoi(*stringVal);
        if (m_type == Type::DOUBLE)
            return static_cast<int>(doubleVal);
        throw std::runtime_error("Cannot cast CVariant to int32_t");
    }

    uint32_t toUInt32() const {
        if (m_type == Type::UINT32)
            return uint32Val;
        if (m_type == Type::STRING && stoi(*stringVal) >= 0 && isNumeric(*stringVal))
            return stoi(*stringVal);

        if (m_type == Type::DOUBLE && static_cast<int>(doubleVal >= 0))
            return static_cast<int>(doubleVal);
        throw std::runtime_error("Cannot cast CVariant to uint32_t");
    }

    int64_t toInt64() const {
        if (m_type == Type::INT64)
            return int64Val;
        throw std::runtime_error("Cannot cast CVariant to int64_t");
    }

    uint64_t toUInt64() const {
        if (m_type == Type::UINT64)
            return uint64Val;
        throw std::runtime_error("Cannot cast CVariant to uint64_t");
    }

    float toFloat() const {
        if (m_type == Type::FLOAT)
            return floatVal;
        if (m_type == Type::STRING && isNumeric(*stringVal))
            return stof(*stringVal);
        if (m_type == Type::DOUBLE)
            static_cast<float>(doubleVal);
        if (m_type == Type::INT32)
            return float(int32Val);
        throw std::runtime_error("Cannot cast CVariant to float");
    }

    double toDouble() const {
        if (m_type == Type::DOUBLE)
            return doubleVal;
        if (m_type == Type::STRING && isNumeric(*stringVal))
            return stoi(*stringVal);
        if (m_type == Type::INT32)
            {double c = int32Val;
            return c;}
        throw std::runtime_error("Cannot cast CVariant to double");
    }

    std::string toString() const {
        if (m_type == Type::STRING)
            return *stringVal;
        if (m_type == Type::INT32)
            return to_string(int32Val);
        if (m_type == Type::FLOAT)
            return to_string(floatVal);
        if (m_type == Type::DOUBLE)
            return to_string(doubleVal);
        throw std::runtime_error("Cannot cast CVariant to std::string");
    }

    std::wstring toWideString() const {
        if (m_type == Type::WIDE_STRING)
            return *wideStringVal;
        throw std::runtime_error("Cannot cast CVariant to std::wstring");
    }

    std::vector<CVariant> toArray() const {
        if (m_type == Type::ARRAY)
            return m_array;
        throw std::runtime_error("Cannot cast CVariant to std::vector<CVariant>");
    }

    std::map<std::string, CVariant> toMap() const {
        if (m_type == Type::MAP)
            return m_map;
        throw std::runtime_error("Cannot cast CVariant to std::map<std::string, CVariant>");
    }

    std::vector<unsigned char> toByteArray() const {
        if (m_type == Type::BYTE_ARRAY)
            return m_byteArray;
        throw std::runtime_error("Cannot cast CVariant to std::vector<unsigned char>");
    }

    // Arithmetic operators
    CVariant operator+(const CVariant& other) const {
        if (m_type == Type::INT32 && other.m_type == Type::INT32)
            return int32Val + other.int32Val;
        if (m_type == Type::UINT32 && other.m_type == Type::UINT32)
            return uint32Val + other.uint32Val;
        if (m_type == Type::INT32 && other.m_type == Type::FLOAT)
            return int32Val + other.floatVal;
        if (m_type == Type::FLOAT && other.m_type == Type::INT32)
            return floatVal + other.int32Val;
        if (m_type == Type::INT32 && other.m_type == Type::DOUBLE)
            return int32Val + other.doubleVal;
        if (m_type == Type::DOUBLE && other.m_type == Type::INT32)
            return doubleVal + other.int32Val;
        if (m_type == Type::FLOAT && other.m_type == Type::FLOAT)
            return floatVal + other.floatVal;
        if (m_type == Type::FLOAT && other.m_type == Type::DOUBLE)
            return floatVal + other.doubleVal;
        if (m_type == Type::DOUBLE && other.m_type == Type::FLOAT)
            return doubleVal + other.floatVal;
        if (m_type == Type::DOUBLE && other.m_type == Type::DOUBLE)
            return doubleVal + other.doubleVal;
        throw std::runtime_error("Cannot perform addition on CVariant");
    }



    CVariant operator-(const CVariant& other) const {
        if (m_type == Type::INT32 && other.m_type == Type::INT32)
            return int32Val - other.int32Val;
        if (m_type == Type::UINT32 && other.m_type == Type::UINT32)
            return uint32Val - other.uint32Val;
        if (m_type == Type::INT32 && other.m_type == Type::FLOAT)
            return int32Val - other.floatVal;
        if (m_type == Type::FLOAT && other.m_type == Type::INT32)
            return floatVal - other.int32Val;
        if (m_type == Type::INT32 && other.m_type == Type::DOUBLE)
            return int32Val - other.doubleVal;
        if (m_type == Type::DOUBLE && other.m_type == Type::INT32)
            return doubleVal - other.int32Val;
        if (m_type == Type::FLOAT && other.m_type == Type::FLOAT)
            return floatVal - other.floatVal;
        if (m_type == Type::FLOAT && other.m_type == Type::DOUBLE)
            return floatVal - other.doubleVal;
        if (m_type == Type::DOUBLE && other.m_type == Type::FLOAT)
            return doubleVal - other.floatVal;
        if (m_type == Type::DOUBLE && other.m_type == Type::DOUBLE)
            return doubleVal - other.doubleVal;
        throw std::runtime_error("Cannot perform subtraction on CVariant");
    }

    CVariant operator*(const CVariant& other) const {
        if (m_type == Type::INT32 && other.m_type == Type::INT32)
            return int32Val * other.int32Val;
        if (m_type == Type::UINT32 && other.m_type == Type::UINT32)
            return uint32Val * other.uint32Val;
        if (m_type == Type::INT32 && other.m_type == Type::FLOAT)
            return int32Val * other.floatVal;
        if (m_type == Type::FLOAT && other.m_type == Type::INT32)
            return floatVal * other.int32Val;
        if (m_type == Type::INT32 && other.m_type == Type::DOUBLE)
            return int32Val * other.doubleVal;
        if (m_type == Type::DOUBLE && other.m_type == Type::INT32)
            return doubleVal * other.int32Val;
        if (m_type == Type::FLOAT && other.m_type == Type::FLOAT)
            return floatVal * other.floatVal;
        if (m_type == Type::FLOAT && other.m_type == Type::DOUBLE)
            return floatVal * other.doubleVal;
        if (m_type == Type::DOUBLE && other.m_type == Type::FLOAT)
            return doubleVal * other.floatVal;
        if (m_type == Type::DOUBLE && other.m_type == Type::DOUBLE)
            return doubleVal * other.doubleVal;
        throw std::runtime_error("Cannot perform multiplication on CVariant");
    }

    CVariant operator/(const CVariant& other) const {
        if (m_type == Type::INT32 && other.m_type == Type::INT32)
            return int32Val / other.int32Val;
        if (m_type == Type::UINT32 && other.m_type == Type::UINT32)
            return uint32Val / other.uint32Val;
        if (m_type == Type::INT32 && other.m_type == Type::FLOAT)
            return int32Val / other.floatVal;
        if (m_type == Type::FLOAT && other.m_type == Type::INT32)
            return floatVal / other.int32Val;
        if (m_type == Type::INT32 && other.m_type == Type::DOUBLE)
            return int32Val / other.doubleVal;
        if (m_type == Type::DOUBLE && other.m_type == Type::INT32)
            return doubleVal / other.int32Val;
        if (m_type == Type::FLOAT && other.m_type == Type::FLOAT)
            return floatVal / other.floatVal;
        if (m_type == Type::FLOAT && other.m_type == Type::DOUBLE)
            return floatVal / other.doubleVal;
        if (m_type == Type::DOUBLE && other.m_type == Type::FLOAT)
            return doubleVal / other.floatVal;
        if (m_type == Type::DOUBLE && other.m_type == Type::DOUBLE)
            return doubleVal / other.doubleVal;
        throw std::runtime_error("Cannot perform division on CVariant");
    }

    CVariant operator%(const CVariant& other) const {
        if (m_type == Type::INT32 && other.m_type == Type::INT32)
            return int32Val % other.int32Val;
        if (m_type == Type::UINT32 && other.m_type == Type::UINT32)
            return uint32Val % other.uint32Val;
        throw std::runtime_error("Cannot perform modulus on CVariant");
    }
};
std::ostream& operator<<(std::ostream& os, const CVariant& variant) {
    // Write the appropriate value based on the variant type
    switch (variant.m_type) {
        case CVariant::Type::INT8:
            os << variant.int8Val;
            break;
        case CVariant::Type::UINT8:
            os << variant.uint8Val;
            break;
        case CVariant::Type::INT16:
            os << variant.int16Val;
            break;
        case CVariant::Type::UINT16:
            os << variant.uint16Val;
            break;
        case CVariant::Type::INT32:
            os << variant.int32Val;
            break;
        case CVariant::Type::UINT32:
            os << variant.uint32Val;
            break;
        case CVariant::Type::INT64:
            os << variant.int64Val;
            break;
        case CVariant::Type::UINT64:
            os << variant.uint64Val;
            break;
        case CVariant::Type::FLOAT:
            os << variant.floatVal;
            break;
        case CVariant::Type::DOUBLE:
            os << variant.doubleVal;
            break;
        case CVariant::Type::STRING:
            os << *(variant.stringVal);
            break;
        case CVariant::Type::WIDE_STRING:
            os << *(variant.wideStringVal);
            break;
        case CVariant::Type::ARRAY:
            // Handle arrays as desired, for example:
            os << "[";
            for (const auto& item : variant.m_array) {
                os << item << ", ";
            }
            os << "]";
            break;
        case CVariant::Type::MAP:
            // Handle maps as desired, for example:
            os << "{";
            for (const auto& entry : variant.m_map) {
                os << entry.first << ": " << entry.second << ", ";
            }
            os << "}";
            break;
        case CVariant::Type::BYTE_ARRAY:
            // Handle byte arrays as desired, for example:
            os << "[";
            for (const auto& byte : variant.m_byteArray) {
                os << static_cast<int>(byte) << ", ";
            }
            os << "]";
            break;
    }

    return os;
}



int main() {
    try{
        CVariant a(10);
        CVariant b(3);
        CVariant c = a + b;
        CVariant m("abc");
        CVariant n = "BP";
        CVariant d = "243";
        CVariant z = "452.4";
        CVariant l = "14.5j";
        CVariant h(2.5f);

        cout << a.toInt32() << endl;
        cout << b.toString() << endl;
        cout << c.toDouble() << endl;
        cout << n.toInt16() << endl;
        cout << m.toInt64() << endl;
        cout << d.toString() << endl;
        cout << z.toFloat() << endl;
        cout << h.toInt32() << endl;
        cout << l.toString() << endl;
    }catch (const std::exception& ex) {
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }
    return 0;
}
