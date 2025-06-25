#include "Singleton.h"
#include <iostream>

class Database {
public:
    void Query(const std::string& sql) {
        std::cout << "Executing: " << sql << std::endl;
    }
private:
    Database() = default;
    friend struct DefaultCreation<Database>;
};

using DBSingleton = Singleton<
    Database,
    DefaultCreation,
    ClassLevelLockable,
    DefaultLifetime
>;

int main() {
    DBSingleton::Instance().Query("SELECT * FROM users");
    return 0;
}