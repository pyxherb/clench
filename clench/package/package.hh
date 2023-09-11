#ifndef _CLENCH_PACKAGE_PACKAGE_H_
#define _CLENCH_PACKAGE_PACKAGE_H_

#include <stdexcept>

#include "base/texture.h"

namespace clench {
	namespace package {
		class PackageScope {
		protected:
			std::string _name;
			std::shared_ptr<PackageScope> _subscope;

		public:
			inline PackageScope(std::string name) {
				_name = name;
			}
			virtual inline ~PackageScope() {}
			virtual std::string getName() {
				return _name;
			}
			virtual std::shared_ptr<PackageScope> getSubscope() {
				return _subscope;
			}
		};
		class PackagePath {
		protected:
			std::shared_ptr<PackageScope> _scopes;

		public:
			inline PackagePath(std::shared_ptr<PackageScope> scopes) { _scopes = scopes; }
			inline PackagePath(std::string path) {
				auto i = path.find('.');
			}
			virtual inline ~PackagePath() {}
			virtual std::shared_ptr<PackageScope> getScopes() {
				return _scopes;
			}
		};

		class IPackage {
		public:
			virtual inline ~IPackage() {}
			virtual std::string getDisplayName() = 0;
			virtual std::string getAuthor() = 0;
		};
	}
}

namespace std {
	inline std::string to_string(clench::package::PackagePath& p) {
		auto x = p.getScopes();
		std::string path = x->getName();
		while ((x = x->getSubscope()))
			path += '.' + x->getName();
		return path;
	}
	inline std::string to_string(clench::package::PackageScope& p) {
		std::string path = p.getName();
		auto x = p.getSubscope();
		while (x) {
			path += '.' + x->getName();
			x = x->getSubscope();
		}
		return path;
	}
}

#endif
