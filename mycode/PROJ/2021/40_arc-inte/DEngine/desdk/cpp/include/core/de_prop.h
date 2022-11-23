/**
 * @file de_prop.h
 * @brief support property set/get and serialize/deserialize
 * @author DeSdk Team
 * @date 2019/12/15
 * @copyright Intellif
 */

#ifndef DE_PROP_H_
#define DE_PROP_H_

#include "de_logging.h"
#include "de_serializer.h"
#include <map>
#include <vector>
#include <limits>
#include <iostream>
#include <string>


namespace de {

    /**
     * @brief Class used to set/get attribute, supporting serialization/de-serialization
     */
	class Prop {
	public:
		Prop() {};
		~Prop() {};

		/**
		 * @brief Set attribute with int type value
		 * @param _key attribute name
		 * @param _val attribute value
		 * @return true if set attribute succed, false otherwise
		 * @note This function is expected to be called by users of a class. \
		 * Designer of the class should call SetDefault(_key, ...) in advance, \
		 * otherwise user call Set(_key, ...) will return false.
		 */
		bool Set(const std::string& _key, const int _val) {
			CHECK(int_paras_.count(_key) > 0) << "key " << _key << " not exist! Call SetDefault before Set.";

            auto vs = int_paras_[_key];
			CHECK ((vs.low <= _val) && (vs.upper >= _val)) << _key << ":" << _val << " exceeds range ["
                << vs.low << ", " << vs.upper << "]";
			int_paras_[_key].value = _val;
			return true;
		}

		/**
		 * @brief Set attribute with double type value
		 * @param _key attribute name
		 * @param _val attribute value
		 * @return true if set attribute succed, false otherwise
		 * @note This function is expected to be called by users of a class. \
		 * Designer of the class should call SetDefault(_key, ...) in advance, \
		 * otherwise user call Set(_key, ...) will return false.
		 */
		bool Set(const std::string& _key, const double _val) {
		    CHECK(double_paras_.count(_key) > 0) << "key " << _key << " not exist! Call SetDefault before Set.";

		    auto vs = double_paras_[_key];
		    CHECK ((vs.low <= _val) && (vs.upper >= _val)) << _key << ":" << _val << " exceeds range ["
		        << vs.low << ", " << vs.upper << "]";
		    double_paras_[_key].value = _val;
		    return true;
		}

		/**
		 * @brief Set attribute with string type value
		 * @param _key attribute name
		 * @param _val attribute value
		 * @return true if set attribute succed, false otherwise
		 * @note This function is expected to be called by users of a class. \
		 * Designer of the class should call SetDefault(_key, ...) in advance, \
		 * otherwise user call Set(_key, ...) will return false.
		 */
		bool Set(const std::string& _key, const std::string& _val) {
			CHECK (string_paras_.count(_key) > 0) << "key " << _key << " not exist! Call SetDefault before Set.";

			string_paras_[_key] = _val;
			return true;
		}

		/**
		 * @brief Get value of an attribute with int type
		 * @param _key attribute name
		 * @param[out] _val attribute value
		 * @return true if attribute with name _key exists, false otherwise
		 */
		bool Get(const std::string& _key, int& _val) const {
			if (int_paras_.count(_key)) {
				_val = int_paras_.at(_key).value;
				return true;
			}
			LOG(WARNING)<< "int_paras has no attribute " << _key;
			return false;
		}

		/**
		 * @brief Get value of an attribute with double type
		 * @param _key attribute name
		 * @param[out] _val attribute value
		 * @return true if attribute with name _key exists, false otherwise
		 */
		bool Get(const std::string& _key, double& _val) const {
			if (double_paras_.count(_key)) {
				_val = double_paras_.at(_key).value;
				return true;
			}
			LOG(WARNING)<< "double_paras has no attribute " << _key;
			return false;
		}

		/**
		 * @brief Get value of an attribute with string type
		 * @param _key attribute name
		 * @param[out] _val attribute value
		 * @return true if attribute with name _key exists, false otherwise
		 */
		bool Get(const std::string& _key, std::string& _val) const {
			if (string_paras_.count(_key)) {
				_val = string_paras_.at(_key);
				return true;
			}
			LOG(WARNING)<< "string_paras has no attribute " << _key;
			return false;
		}

	public:
		template<typename T>
        /**
         * @brief General value type
         */
		struct Value {
			T value;
			T upper;
			T low;
		};

		/**< name and value of attributes with int type */
		std::map<std::string, Value<int>> int_paras_;
		/**< name and value of attributes with double type */
		std::map<std::string, Value<double>> double_paras_;
		/**< name and value of attributes with string type */
		std::map<std::string, std::string> string_paras_;

		/**
		 * @brief Set default value of attribute with int type
		 * @param _key attribute name
		 * @param _val attribute value
		 * @param _low lower bound value of attribute with name _key
		 * @param _upper upper bound value of attribute with name _key
		 * @note This function is expected to be called by designer of a class during the design phase.
		 */
		void SetDefault(const std::string& _key, const int _val,
			const int _low = std::numeric_limits<int>::min(),
			const int _upper = std::numeric_limits<int>::max()) {
			int_paras_[_key].value = _val;
			int_paras_[_key].low = _low;
			int_paras_[_key].upper = _upper;
		}

		/**
		 * @brief Set default value of attribute with double type
		 * @param _key attribute name
		 * @param _val attribute value
		 * @param _low lower bound value of attribute with name _key
		 * @param _upper upper bound value of attribute with name _key
		 * @note This function is expected to be called by designer of a class during the design phase.
		 */
		void SetDefault(const std::string& _key, const double _val,
		    const double _low = std::numeric_limits<double>::lowest(),
		    const double _upper = std::numeric_limits<double>::max()) {
		    double_paras_[_key].value = _val;
		    double_paras_[_key].low = _low;
		    double_paras_[_key].upper = _upper;
		}

		/**
		 * @brief Set default value of attribute with string type
		 * @param _key attribute name
		 * @param _val attribute value
		 * @note This function is expected to be called by designer of a class during the design phase.
		 */
		void SetDefault(const std::string& _key, const std::string _val) {
			string_paras_[_key] = _val;
		}

		/**
		 * @brief Output overload of class Prop
		 * @param[in,out] os    reference of an output stream
		 * @param[in] prop      const reference of a Prop class object
		 * @return  reference of the output stream
		 */
		friend std::ostream& operator<<(std::ostream& os, const Prop& prop);
	};

	inline std::ostream& operator<<(std::ostream& os, const Prop& prop) {
		os << " properties = " << std::endl;
		for (auto it = prop.int_paras_.begin(); it != prop.int_paras_.end(); ++it) {
			os << "key = " << it->first << ", val = " << it->second.value << std::endl;
		}

		for (auto it = prop.double_paras_.begin(); it != prop.double_paras_.end(); ++it) {
			os << "key = " << it->first << ", val = " << it->second.value << std::endl;
		}

		for (auto it = prop.string_paras_.begin(); it != prop.string_paras_.end(); ++it) {
			os << "key = " << it->first << ", val = " << it->second << std::endl;
		}
		return os;
	}
}

namespace de {
namespace serializer {
	using map_string_value_int = std::map<std::string, Prop::Value<int>> ;
	using map_string_value_double = std::map<std::string, Prop::Value<double>>;
	using map_string_value_string = std::map<std::string, std::string>;

	STRUCT_SERIALIZE_3(Prop,
			map_string_value_int, int_paras_,
			map_string_value_double, double_paras_,
			map_string_value_string, string_paras_);
}
}



#endif

