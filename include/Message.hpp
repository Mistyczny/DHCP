#pragma once
#include <cstring>
#include <iostream>
#include <vector>

template <typename T> struct Message {
    std::vector<char> body;

    [[nodiscard]] size_t size() const { return this->body.size(); }

    friend std::ostream& operator<<(std::ostream& os, const Message<T>& message) {
        os << "Message: " << message.body.data() << std::endl;
        return os;
    }

    template <typename DataType> friend Message<T>& operator<<(Message<T>& message, const DataType& data) {
        // Check that the type of the data being pushed is trivially copyable
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

        // Cache current size of vector, as this will be the point we insert the data
        size_t i = message.body.size();

        // Resize the vector by the size of the data being pushed
        message.body.resize(message.body.size() + sizeof(DataType));

        // Physically copy the d   ata into the newly allocated vector space
        std::memcpy(message.body.data() + i, &data, sizeof(DataType));

        return message;
    }

    // Pulls any POD-like data form the message buffer
    template<typename DataType>
    friend Message<T>& operator >> (Message<T>& message, DataType& data) {
        // Check that the type of the data being pushed is trivially copyable
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");
        // Physically copy the data from the vector into the user variable
        std::memcpy(&data, message.body.data(), sizeof(DataType));
        return message;
    }
};