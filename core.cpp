#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <cctype>
#include <conio.h>
#include "core.h"

using namespace std;

string toLower(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

string prompt(string message)
{
    cout << message;
    string inp;
    cin >> inp;
    cin.clear();
    cin.ignore(5000, '\n');
    return toLower(inp);
}

string inputPassword()
{
    string pass;
    char c;
    c = _getch();
    while (c != 13)
    {
        pass += c;
        cout << "*";
        c = _getch();
    }
    return pass;
}

Product::Product(string name, string price)
{
    productCounter++;
    this->uid = productCounter;
    this->name = name;
    this->price = price;
}
Product::Product(int uid, string name, string price)
{
    this->uid = uid;
    this->name = name;
    this->price = price;
}

void Product::updateProduct(string name, string price)
{
    this->name = name;
    this->price = price;
}

Order::Order(int productUid, string status, string username)
{
    orderCounter++;
    this->uid = orderCounter;
    this->productUid = productUid;
    this->status = status;
    this->username = username;
}
Order::Order(int uid, int productUid, string status, string username)
{
    this->uid = uid;
    this->productUid = productUid;
    this->status = status;
    this->username = username;
}

void Order::updateStatus(string status)
{
    this->status = status;
}

Customer::Customer(string username, string password)
{
    this->username = toLower(username);
    this->password = password;
}

bool Customer::matchPassword(string password)
{
    return this->password == password;
}

vector<Customer> Customers = {};
vector<Product> Products = {};
vector<Order> Orders = {};

string title, note;
string customersStorageLocation = "customers";
string productsStorageLocation = "products";
string ordersStorageLocation = "orders";
string appDataStorageLocation = "appData";
int Product::productCounter = 10;
int Order::orderCounter = 10;

string RED_TEXT = "\033[38;2;255;69;0m";
string WHITE_TEXT = "\033[38;2;255;250;250m";
string GREEN_TEXT = "\033[38;2;50;205;50m";

void saveCustomers()
{
    fstream file;
    file.open(customersStorageLocation, ios::out | ios::binary);
    for (auto customer : Customers)
    {
        file << customer.username << " ";
        file << customer.password << " ";
        file << endl;
    }
}

void loadCustomers()
{
    fstream file;
    file.open(customersStorageLocation, ios::in | ios::binary);
    if (!file)
    {
        return;
    }
    Customers.clear();
    while (!file.eof())
    {
        Customer customer;
        file >> customer.username;
        file >> customer.password;
        if (customer.username != "")
            Customers.push_back(customer);
    }
}

void saveProducts()
{
    fstream file;
    file.open(productsStorageLocation, ios::out | ios::binary);
    for (auto product : Products)
    {
        if (product.uid > 0)
        {
            file << product.uid << " ";
            file << product.name << " ";
            file << product.price << " ";
            file << endl;
        }
    }
}

void loadProducts()
{
    fstream file;
    file.open(productsStorageLocation, ios::in);
    if (!file)
    {
        return;
    }
    Products.clear();
    while (!file.eof())
    {
        Product product;
        file >> product.uid;
        file >> product.name;
        file >> product.price;
        if (product.name != "")
            Products.push_back(product);
    }
}

void saveOrders()
{
    fstream file;
    file.open(ordersStorageLocation, ios::out | ios::binary);
    for (auto order : Orders)
    {
        file << order.uid << " ";
        file << order.productUid << " ";
        file << order.status << " ";
        file << order.username << " ";
        file << endl;
    }
}

void loadOrders()
{
    fstream file;
    file.open(ordersStorageLocation, ios::in);
    if (!file)
    {
        return;
    }
    Orders.clear();
    while (!file.eof())
    {
        Order order;
        file >> order.uid;
        file >> order.productUid;
        file >> order.status;
        file >> order.username;
        if (order.username != "")
            Orders.push_back(order);
    }
}

void saveState()
{
    saveCustomers();
    saveProducts();
    saveOrders();

    fstream file;
    file.open(appDataStorageLocation, ios::out);
    file << Product::productCounter << " ";
    file << Order::orderCounter << " ";
    file << endl;
}

void loadState()
{
    loadCustomers();
    loadProducts();
    loadOrders();

    fstream file;
    file.open(appDataStorageLocation, ios::in);
    if (!file)
    {
        return;
    }
    while (!file.eof())
    {
        file >> Product::productCounter;
        file >> Order::orderCounter;
    }
}

bool orderExists(int uid)
{
    for (auto order : Orders)
        if (order.uid == uid)
            return true;
    return false;
}

bool productExists(int uid)
{
    for (auto product : Products)
        if (product.uid == uid)
            return true;
    return false;
}

bool customerExists(string username)
{
    for (auto customer : Customers)
        if (customer.username == username)
            return true;
    return false;
}

Order &getOrder(int uid)
{
    for (auto &order : Orders)
        if (order.uid == uid)
            return order;
    Order dummy = Order();
    Order &order = dummy;
    return order;
}

Product &getProduct(int uid)
{
    for (auto &product : Products)
        if (product.uid == uid)
            return product;
    Product dummy = Product();
    Product &product = dummy;
    return product;
}

Customer &getCustomer(string username)
{
    for (auto &customer : Customers)
        if (customer.username == username)
            return customer;
    Customer dummy = Customer();
    Customer &customer = dummy;
    return customer;
}

void addNewProduct()
{
    title = "Create New Product";
    note = "";
    while (true)
    {
        system("cls");
        cout << "\n[ " << title << " ]\n";
        cout << "\n"
             << note << WHITE_TEXT + "\n\n";

        string name, price;

        cout << "Product Name: ";
        getline(cin, name);
        replace(name.begin(), name.end(), ' ', '_');
        cout << "Product Price: ";
        cin >> price;
        price = "Rs." + price;

        Product product = Product(name, price);

        Products.push_back(product);

        note = GREEN_TEXT + "New Product Created!";
        title = "Products";
        saveState();
        return;
    }
}

void deleteProduct(int uid)
{
    for (int i = 0; i < Products.size(); i++)
    {
        if (Products[i].uid == uid)
        {
            Products.erase(Products.begin() + i);
            break;
        }
    }
    saveState();
    for (int i = 0; i < Orders.size(); i++)
    {
        if (Orders[i].productUid == uid)
        {
            Orders.erase(Orders.begin() + i);
            break;
        }
    }
    saveState();
    note = RED_TEXT + "Product Deleted!";
}

void updateProduct(Product &product)
{
    title = "Edit Product";
    note = "";
    while (true)
    {
        loadState();
        system("cls");
        cout << "\n[ " << title << " ]\n";
        cout << "\n"
             << note << WHITE_TEXT + "\n\n";
        cout << setw(11) << left << "ProductID"
             << setw(15) << left << "Product"
             << setw(10) << left << "Price";
        cout << "\n\n";
        cout << setw(11) << left << product.uid
             << setw(15) << left << product.name
             << setw(10) << left << product.price;
        cout << "\n";

        string name, price;

        cout << "\n\nNew Name: ";
        getline(cin, name);
        replace(name.begin(), name.end(), ' ', '_');
        cout << "New Price: ";
        cin >> price;
        price = "Rs." + price;

        product.updateProduct(name, price);

        note = GREEN_TEXT + "Product Updated!";
        title = "Products";
        saveState();
        return;
    }
}

void selectProductOptions(Product &product)
{
    title = "Product";
    note = "Make changes to Product";
    while (true)
    {
        loadState();
        system("cls");
        cout << "\n[ " << title << " ]\n";
        cout << "\n"
             << note << WHITE_TEXT + "\n\n";
        cout << setw(11) << left << "ProductID"
             << setw(15) << left << "Product"
             << setw(10) << left << "Price";
        cout << "\n\n";
        cout << setw(11) << left << product.uid
             << setw(15) << left << product.name
             << setw(10) << left << product.price;
        cout << "\n";
        cout << "\n\n1. Edit Product\n";
        cout << "2. Delete Product\n";
        cout << "0. Exit\n";

        string option = prompt("\n\n> ");

        if (option == "0")
        {
            title = "Products";
            note = "Add or edit products";
            return;
        }
        else if (option == "1")
        {
            updateProduct(product);
            return;
        }
        else if (option == "2")
        {
            deleteProduct(product.uid);
            return;
        }
        else
        {
            note = RED_TEXT + "Invalid choice. Try again.";
        }
    }
}

void updateProducts()
{
    title = "Products";
    note = "Add or edit products";
    while (true)
    {
        loadState();
        system("cls");
        cout << "\n[ " << title << " ]\n";
        cout << "\n"
             << note << WHITE_TEXT + "\n\n";
        cout << setw(11) << left << "ProductID"
             << setw(15) << left << "Product"
             << setw(10) << left << "Price";
        cout << "\n\n";
        if (Products.empty())
        {
            cout << "No Products here!\n";
        }
        else
        {
            for (auto &product : Products)
            {
                cout << setw(11) << left << product.uid
                     << setw(15) << left << product.name
                     << setw(10) << left << product.price;
                cout << "\n";
            }
        }
        cout << "\n\n1. Add New Product\n";
        cout << "0. Exit\n";

        string option = prompt("\n\n> ");

        if (option == "0")
        {
            title = "Dashboard";
            note = "";
            return;
        }
        else if (option == "1")
        {
            addNewProduct();
        }
        else
        {
            try
            {
                int uid = stoi(option);
                if (productExists(stoi(option)))
                {
                    Product &product = getProduct(uid);
                    selectProductOptions(product);
                }
                else
                {
                    note = RED_TEXT + "Invalid choice. Try again.";
                }
            }
            catch (...)
            {
                note = RED_TEXT + "Invalid choice. Try again.";
            }
        }
    }
}

void updateOrder(Order &order)
{
    title = "Update Order";
    note = "";
    while (true)
    {
        loadState();
        system("cls");
        cout << "\n[ " << title << " ]\n";
        cout << "\n"
             << note << WHITE_TEXT + "\n\n";
        cout << setw(10) << left << "OrderID"
             << setw(15) << left << "Product"
             << setw(10) << left << "Price"
             << setw(12) << left << "Status"
             << setw(15) << left << "Customer";
        cout << "\n\n";
        Product &product = getProduct(order.productUid);
        cout << setw(10) << left << order.uid
             << setw(15) << left << product.name
             << setw(10) << left << product.price
             << setw(12) << left << order.status
             << setw(15) << left << order.username;
        cout << "\n";

        cout << "\n\n1. Mark as Delivered\n";
        cout << "2. Reject Order\n";
        cout << "0. Exit\n";

        string option = prompt("\n\n> ");

        if (option == "0")
        {
            return;
        }
        else if (option == "1")
        {
            order.updateStatus("Delivered");
            note = GREEN_TEXT + "Order status changed to Delivered!";
            saveState();
            return;
        }
        else if (option == "2")
        {
            order.updateStatus("Rejected");
            note = RED_TEXT + "Order status changed to Rejected!";
            saveState();
            return;
        }
        else
        {
            note = RED_TEXT + "Invalid choice. Try again.";
        }
    }
}

void updateCustomerOrder(Order &order)
{
    title = "Order Details";
    note = "";
    while (true)
    {
        loadState();
        system("cls");
        cout << "\n[ " << title << " ]\n";
        cout << "\n"
             << note << WHITE_TEXT + "\n\n";
        cout << setw(10) << left << "OrderID"
             << setw(15) << left << "Product"
             << setw(10) << left << "Price"
             << setw(12) << left << "Status";
        cout << "\n\n";
        Product &product = getProduct(order.productUid);
        cout << setw(10) << left << order.uid
             << setw(15) << left << product.name
             << setw(10) << left << product.price
             << setw(12) << left << order.status;
        cout << "\n";

        cout << "\n\n1. Cancel Order\n";
        cout << "0. Exit\n";

        string option = prompt("\n\n> ");

        if (option == "0")
        {
            return;
        }
        else if (option == "1")
        {
            for (int i = 0; i < Orders.size(); i++)
            {
                if (Orders[i].uid == order.uid)
                {
                    // delete &Products[i];
                    Orders.erase(Orders.begin() + i);
                    break;
                }
            }
            note = GREEN_TEXT + "Order Canceled!";
            saveState();
            return;
        }
        else
        {
            note = RED_TEXT + "Invalid choice. Try again.";
        }
    }
}

void viewOrders()
{
    title = "Orders";
    note = "Select Order ID to modify";
    while (true)
    {
        loadState();
        system("cls");
        cout << "\n[ " << title << " ]\n";
        cout << "\n"
             << note << WHITE_TEXT + "\n\n";
        cout << setw(10) << left << "OrderID"
             << setw(15) << left << "Product"
             << setw(10) << left << "Price"
             << setw(12) << left << "Status"
             << setw(15) << left << "Customer";
        cout << "\n\n";
        if (Orders.empty())
        {
            cout << "No Orders here!\n";
        }
        else
        {
            for (auto &order : Orders)
            {
                Product &product = getProduct(order.productUid);
                cout << setw(10) << left << order.uid
                     << setw(15) << left << product.name
                     << setw(10) << left << product.price
                     << setw(12) << left << order.status
                     << setw(15) << left << order.username;
                cout << "\n";
            }
        }
        cout << "\n\nPress 0 to Exit\n";

        string option = prompt("\n\n> ");

        if (option == "0")
        {
            title = "Dashboard";
            note = "";
            return;
        }
        else
        {
            try
            {
                int uid = stoi(option);
                if (orderExists(stoi(option)))
                {
                    Order &order = getOrder(uid);
                    if (order.status == "Delivered")
                    {
                        note = GREEN_TEXT + "Sorry order has been delivered and can't be modified.";
                    }
                    else if (order.status == "Rejected")
                    {
                        note = RED_TEXT + "Sorry order has been rejected and no longer exist.";
                    }
                    else
                    {
                        updateOrder(order);
                    }
                }
                else
                {
                    note = RED_TEXT + "Invalid choice. Try again.";
                }
            }
            catch (...)
            {
                note = RED_TEXT + "Invalid choice. Try again.";
            }
        }
    }
}

void viewCustomers()
{
    title = "Customers";
    note = "";
    while (true)
    {
        loadState();
        system("cls");
        cout << "\n[ " << title << " ]\n";
        cout << "\n"
             << note << WHITE_TEXT + "\n";
        cout << setw(15) << left << "Username"
             << setw(15) << left << "Orders";
        cout << "\n\n";
        for (auto customer : Customers)
        {
            int count = 0;
            for (auto order : Orders)
            {
                if (order.username == customer.username)
                    count++;
            }
            cout << setw(15) << left << customer.username;
            cout << setw(15) << left << count;
            cout << "\n";
        }
        cout << "\n\nPress 0 to Exit";

        string option = prompt("\n\n> ");

        if (option == "0")
        {
            return;
        }
        else
        {
            note = RED_TEXT + "Invalid choice. Try again.";
        }
    }
}

void adminDashboard()
{
    title = "Admin Dashboard";
    while (true)
    {
        system("cls");
        cout << "\n[ " << title << " ]\n";
        cout << "\n"
             << note << WHITE_TEXT + "\n";
        cout << "\n1. Update Products";
        cout << "\n2. View Orders";
        cout << "\n3. View Customers";
        cout << "\n0. Logout";

        string option = prompt("\n\n> ");

        if (option == "0")
        {
            title = "Welcome to Cafeteria!";
            note = GREEN_TEXT + "Logged Out Successfully!";
            return;
        }
        else if (option == "1")
        {
            updateProducts();
        }
        else if (option == "2")
        {
            viewOrders();
        }
        else if (option == "3")
        {
            viewCustomers();
        }
        else
        {
            note = RED_TEXT + "Invalid choice. Try again.";
        }
    }
}

void orderProduct(string username)
{
    title = "Order Coffee";
    note = "Select a Product";
    while (true)
    {
        loadState();
        system("cls");
        cout << "\n[ " << title << " ]\n";
        cout << "\n"
             << note << WHITE_TEXT + "\n\n";
        cout << setw(11) << left << "ProductID"
             << setw(15) << left << "Product"
             << setw(10) << left << "Price";
        cout << "\n\n";
        if (Products.empty())
        {
            cout << "No Products here!\n";
        }
        else
        {
            for (auto &product : Products)
            {
                cout << setw(11) << left << product.uid
                     << setw(15) << left << product.name
                     << setw(10) << left << product.price;
                cout << "\n";
            }
        }
        cout << "\n\n0. Exit";

        string option = prompt("\n\n> ");

        if (option == "0")
        {
            title = "Dashboard";
            note = "";
            return;
        }
        else
        {
            try
            {
                int uid = stoi(option);
                if (productExists(stoi(option)))
                {
                    Product &product = getProduct(uid);
                    note = GREEN_TEXT + "Order Placed!";
                    Order order = Order(product.uid, "Pending", username);
                    Orders.push_back(order);
                    saveState();
                    return;
                }
                else
                {
                    note = RED_TEXT + "Invalid choice. Try again.";
                }
            }
            catch (...)
            {
                note = RED_TEXT + "Invalid choice. Try again.";
            }
        }
    }
}

void customerOrderHistory(string username)
{
    title = "Your Orders";
    note = "Select OrderID to modify";
    while (true)
    {
        loadState();
        system("cls");
        cout << "\n[ " << title << " ]\n";
        cout << "\n"
             << note << WHITE_TEXT + "\n\n";
        cout << setw(10) << left << "OrderID"
             << setw(15) << left << "Product"
             << setw(10) << left << "Price"
             << setw(12) << left << "Status";
        cout << "\n\n";
        if (Orders.empty())
        {
            cout << "No Orders here!\n";
        }
        else
        {
            for (auto &order : Orders)
            {
                if (order.username == username)
                {
                    Product &product = getProduct(order.productUid);
                    cout << setw(10) << left << order.uid
                         << setw(15) << left << product.name
                         << setw(10) << left << product.price
                         << setw(12) << left << order.status;
                    cout << "\n";
                }
            }
        }
        cout << "\n\nPress 0 to Exit\n";

        string option = prompt("\n\n> ");

        if (option == "0")
        {
            title = "Dashboard";
            note = "";
            return;
        }
        else
        {
            try
            {
                int uid = stoi(option);
                if (orderExists(stoi(option)))
                {
                    Order &order = getOrder(uid);
                    if (order.status == "Delivered")
                    {
                        note = GREEN_TEXT + "Sorry order has been delivered and can't be modified.";
                    }
                    else if (order.status == "Rejected")
                    {
                        note = RED_TEXT + "Sorry order has been rejected and no longer exist.";
                    }
                    else
                    {
                        updateCustomerOrder(order);
                    }
                }
                else
                {
                    note = RED_TEXT + "Invalid choice. Try again.";
                }
            }
            catch (...)
            {
                note = RED_TEXT + "Invalid choice. Try again.";
            }
        }
    }
}

void changeCustomerPassword(Customer &customer)
{
    title = "Change Password";
    note = "Create new Password.";

    system("cls");
    cout << "\n[ " << title << " ]\n";
    cout << "\n"
         << note << WHITE_TEXT + "\n";
    cout << "\nNew Password: ";
    string password;
    password = inputPassword();
    if (password.length() < 4)
    {
        note = RED_TEXT + "Password should contain atleast 4 characters";
    }
    else
    {
        customer.password = password;
        saveState();
        title = "Dashboard";
        note = GREEN_TEXT + "Password Updated!";
        return;
    }
}

void customerDashboard(Customer &customer)
{
    title = "Dashboard";
    while (true)
    {
        system("cls");
        cout << "\n[ " << title << " ]\n";
        cout << "\n"
             << note << WHITE_TEXT + "\n";
        cout << "\n1. Order Coffee";
        cout << "\n2. Order History";
        cout << "\n3. Change Password";
        cout << "\n0. Logout";

        string option = prompt("\n\n> ");

        if (option == "0")
        {
            title = "Welcome to Cafeteria!";
            note = GREEN_TEXT + "Logged Out Successfully!";
            return;
        }
        else if (option == "1")
        {
            orderProduct(customer.username);
        }
        else if (option == "2")
        {
            customerOrderHistory(customer.username);
        }
        else if (option == "3")
        {
            changeCustomerPassword(customer);
        }
        else
        {
            note = RED_TEXT + "Invalid choice. Try again.";
        }
    }
}

void createCustomerAccount()
{
    title = "Create Account";
    note = "Create an Username and Password.";
    while (true)
    {
        system("cls");
        cout << "\n[ " << title << " ]\n";
        cout << "\n"
             << note << WHITE_TEXT + "\n";
        cout << "\nUsername: ";
        string username;
        getline(cin, username);
        if (username.length() < 3)
        {
            note = RED_TEXT + "Username should contain atleast 3 characters";
        }
        else
        {
            if (customerExists(username))
            {
                note = RED_TEXT + "Username is already taken. Try a different one.";
            }
            else
            {
                bool flag = true;
                for (auto ch : username)
                {
                    if (isspace(ch))
                    {
                        note = RED_TEXT + "Username should not contain any space.";
                        flag = false;
                        break;
                    }
                }
                if (flag)
                {
                    note = "Create an Username and Password.";
                    while (true)
                    {
                        system("cls");
                        cout << "\n[ " << title << " ]\n";
                        cout << "\n"
                             << note << WHITE_TEXT + "\n";
                        cout << "\nUsername: " << username;
                        cout << "\nPassword: ";
                        string password;
                        password = inputPassword();
                        if (password.length() < 4)
                        {
                            note = RED_TEXT + "Password should contain atleast 4 characters";
                        }
                        else
                        {
                            Customer customer = Customer(username, password);
                            Customers.push_back(customer);
                            saveState();
                            title = "Welcome to Cafeteria!";
                            note = GREEN_TEXT + "Account created successfully!";
                            return;
                        }
                    }
                }
            }
        }
    }
}

void customerLogin()
{
    title = "Customer Login";
    note = "";
    while (true)
    {
        system("cls");
        cout << "\n[ " << title << " ]\n";
        cout << "\n"
             << note << WHITE_TEXT + "\n";
        cout << "\nUsername: ";
        string username;
        cin >> username;
        cout << "Password: ";
        string password = inputPassword();

        if (customerExists(username))
        {
            Customer &customer = getCustomer(username);
            if (customer.matchPassword(password))
            {
                note = GREEN_TEXT + "Welcome! " + username;
                customerDashboard(customer);
                return;
            }
            else
            {
                note = RED_TEXT + "Oops! Either Username or Password is Incorrect!";
            }
        }
        else
        {
            note = RED_TEXT + "Oops! Either Username or Password is Incorrect!";
        }
    }
}

void adminLogin()
{
    title = "Admin Login";
    note = "";
    while (true)
    {
        system("cls");
        cout << "\n[ " << title << " ]\n";
        cout << "\n"
             << note << WHITE_TEXT + "\n";
        cout << "\nUsername: ";
        string username;
        cin >> username;
        cout << "Password: ";
        string password = inputPassword();

        if (toLower(username) == "admin" && password == "1234")
        {
            note = GREEN_TEXT + "Welcome! Admin";
            adminDashboard();
            return;
        }
        else
        {
            note = RED_TEXT + "Oops! Either Username or Password is Incorrect!";
        }
    }
}

void mainMenu()
{
    title = "Welcome to Cafeteria!";
    note = "We server you the best coffee!";
    while (true)
    {
        system("cls");
        cout << "\n[ " << title << " ]\n";
        cout << "\n"
             << note << WHITE_TEXT + "\n";
        cout << "\n1. Existing Customer";
        cout << "\n2. New Customer";
        cout << "\n3. Admin Panel";
        cout << "\n0. Exit";

        string option = prompt("\n\n> ");

        if (option == "0")
        {
            return;
        }
        else if (option == "1")
        {
            customerLogin();
        }
        else if (option == "2")
        {
            createCustomerAccount();
        }
        else if (option == "3")
        {
            adminLogin();
        }
        else
        {
            note = RED_TEXT + "Invalid choice. Try again.";
        }
    }
}
