#ifndef CORE_H
#define CORE_H

using namespace std;

string toLower(string str);

string prompt(string message);

string inputPassword();

class Product
{
public:
    static int productCounter;
    int uid;
    string name;
    string price;

    Product() {}

    Product(string name, string price);

    Product(int uid, string name, string price);

    void updateProduct(string name, string price);
};

class Order
{
public:
    static int orderCounter;
    int uid;
    int productUid;
    string status;
    string username;

    Order() {}

    Order(int productUid, string status, string username);

    Order(int uid, int productUid, string status, string username);

    void updateStatus(string status);
};

class Customer
{

public:
    string password;
    string username;

    Customer() {}

    Customer(string username, string password);

    bool matchPassword(string password);
};

void saveCustomers();

void loadCustomers();

void saveProducts();

void loadProducts();

void saveOrders();

void loadOrders();

void saveState();

void loadState();

bool orderExists(int uid);

bool customerExists(string username);

Order &getOrder(int uid);

Product &getProduct(int uid);

Customer &getCustomer(string username);

void addNewProduct();

void deleteProduct(int uid);

void updateProduct(Product &product);

void selectProductOptions(Product &product);

void updateProducts();

void updateOrder(Order &order);

void updateCustomerOrder(Order &order);

void viewOrders();

void viewCustomers();

void adminDashboard();

void orderProduct(string username);

void customerOrderHistory(string username);

void changeCustomerPassword(Customer &customer);

void customerDashboard(Customer &customer);

void createCustomerAccount();

void customerLogin();

void adminLogin();

void mainMenu();

#endif