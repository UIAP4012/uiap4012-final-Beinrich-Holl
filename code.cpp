#include <bits/stdc++.h>

class Currency {
public:
    Currency(double usdRate = 1.0, double eurRate = 0.85) 
        : usdRate_(usdRate), eurRate_(eurRate) {}

    double ConvertToUSD(double amount, char currency) const {
        switch(currency) {
            case 'E':
                return amount / eurRate_;
            case 'U':
            default:
                return amount;
        }
    }

    double ConvertToEUR(double amount, char currency) const {
        switch(currency) {
            case 'E':
                return amount;
            case 'U':
            default:
                return amount * eurRate_;
        }
    }

private:
    double usdRate_;
    double eurRate_;
};

class Product {
public:
     Product(const std::string &name, double price, char currency = 'U') 
        : name_(name), price_(price), currency_(currency) {}

    std::string GetName() const {
        return name_;
    }

    double GetPrice() const {
        return price_;
    }

    void UpdatePrice(double newPrice) {
        price_ = newPrice;
    }
	
	double GetPriceInUSD(const Currency &currencyConverter) const {
        return currencyConverter.ConvertToUSD(price_, currency_);
    }

    double GetPriceInEUR(const Currency &currencyConverter) const {
        return currencyConverter.ConvertToEUR(price_, currency_);
    }

private:
    std::string name_;
    double price_;
	char currency_;  // 'U' for USD, 'E' for EUR
};

class Employee {
public:
    Employee(std::string username, std::string password)
        : username_(username), password_(password) {}

    std::string GetUsername() const {
        return username_;
    }

    bool CheckPassword(std::string password) const {
        return password_ == password;
    }
	
	std::string GetPassword() const{
		return password_;
	}

private:
    std::string username_;
    std::string password_;
};

class Invoice {
public:
    void AddProduct(const Product& product, int quantity) {
        items_.push_back(std::make_pair(product, quantity));
    }
	
	void SaveToFile(int invoiceId) const {
        std::ofstream file(std::to_string(invoiceId) + ".csv");
        double total = 0;
        for(const auto& pair : items_) {
            double itemTotal = pair.first.GetPrice() * pair.second;
            total += itemTotal;
            file << pair.first.GetName() << "," << pair.second << "," 
                 << pair.first.GetPrice() << "," << itemTotal << "\n";
        }
        file << "Total," << total << "\n";
    }

    void PrintInvoice() const {
        double total = 0;
        std::cout << "Invoice:\n";
        for(const auto& pair : items_) {
            double itemTotal = pair.first.GetPrice() * pair.second;
            total += itemTotal;
            std::cout << pair.first.GetName() << ": " << pair.second << " * "
                      << pair.first.GetPrice() << " = " << itemTotal << "\n";
        }
        std::cout << "Total: " << total << "\n";
    }

private:
    std::vector<std::pair<Product, int>> items_;
};

class Inventory {
public:
    void AddProduct(Product product, int quantity) {
        inventory_[product.GetName()] = std::make_pair(product, quantity);
    }

    void RemoveProduct(std::string name) {
        inventory_.erase(name);
    }

    void UpdateProductPrice(std::string name, double newPrice) {
        if(inventory_.find(name) != inventory_.end()) {
            inventory_[name].first.UpdatePrice(newPrice);
        }
    }

    void DisplayCurrentInventory() const {
        for(const auto &pair : inventory_) {
            std::cout << "Product Name: " << pair.second.first.GetName()
                      << ", Price: " << pair.second.first.GetPrice()
                      << ", Quantity: " << pair.second.second << "\n";
        }
    }

    bool Login(std::string username, std::string password) {
        if(employees_.find(username) != employees_.end()) {
            return employees_[username].CheckPassword(password);
        }

        return false;
    }

    void AddEmployee(Employee employee) {
        employees_[employee.GetUsername()] = employee;
    }
	
	void SaveEmployeesToFile(std::string filename) {
        std::ofstream file(filename);
        for(const auto& pair : employees_) {
            file << pair.second.GetUsername() << " " << pair.second.GetPassword() << "\n";
        }
    }

    void LoadEmployeesFromFile(std::string filename) {
        std::ifstream file(filename);
        std::string username, password;
        while(file >> username >> password) {
            employees_[username] = Employee(username, password);
        }
    }
	
	Invoice CreateInvoice(const std::map<std::string, int>& order) {
        Invoice invoice;
        for(const auto& pair : order) {
            const auto& productPair = inventory_.find(pair.first);
            if(productPair != inventory_.end() && productPair->second.second >= pair.second) {
                invoice.AddProduct(productPair->second.first, pair.second);
                productPair->second.second -= pair.second;
            } else {
                std::cout << "Not enough stock for product: " << pair.first << "\n";
            }
        }
        return invoice;
    }
	
	void SaveInventoryToFile(std::string filename) {
        std::ofstream file(filename);
        for(const auto& pair : inventory_) {
            file << pair.first << " " << pair.second.first.GetPrice() << " " << pair.second.second << "\n";
        }
    }

    void LoadInventoryFromFile(std::string filename) {
        std::ifstream file(filename);
        std::string name;
        double price;
        int quantity;
        while(file >> name >> price >> quantity) {
            inventory_[name] = std::make_pair(Product(name, price), quantity);
        }
    }

private:
    std::map<std::string, std::pair<Product, int>> inventory_;
    std::map<std::string, Employee> employees_;
};

int main() {
    Inventory inventory;
    Product apple("Apple", 1.2);
    Product banana("Banana", 0.8);
    Employee john("john", "john123");
    
    inventory.AddProduct(apple, 100);
    inventory.AddProduct(banana, 150);
    inventory.AddEmployee(john);

    inventory.SaveEmployeesToFile("employees.txt");

    Inventory inventory2;
    inventory2.LoadEmployeesFromFile("employees.txt");

    if(inventory2.Login("john", "john123")) {
        std::map<std::string, int> order = {{"Apple", 2}, {"Banana", 3}};
        Invoice invoice = inventory2.CreateInvoice(order);
        invoice.SaveToFile(123);  // assuming 123 is a unique invoice ID
    } else {
        std::cout << "Login failed!";
    }
    
    return 0;
}
