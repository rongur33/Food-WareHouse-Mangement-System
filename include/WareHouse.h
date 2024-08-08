#pragma once
#include <string>
#include <vector>
using namespace std;
#include <utility>
#include "Order.h"
#include "Customer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers and Actions.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        const vector<BaseAction*> &getActions() const;
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        vector<Order*> &getPendingOrders();
        vector<Order*> &getProcessOrders() ;
        vector<Order*> &getCompletedOrders() ;
        const vector<Customer*> &getCustomers() const;
        const vector<Volunteer*> &getVolunteers() const;
        int getOrderCounter() const;
        void close();
        void open();
        void addCustomer(Customer* _customer);
        int getVolunteerCounter() const;
        int getCustomerCounter() const;
        void deleteFromProcessOrders(int counter);
        void deleteFromPendingOrders(int counter);
        void deleteLimitedVolunteer(int counter,int id);
        ~WareHouse();//destructor
        WareHouse(const WareHouse& other);//copy constructor
        WareHouse& operator=(const WareHouse& other); //Copy Assignment Operator
        WareHouse(WareHouse&& other) ;// Rule of 5: Move Constructor
        WareHouse& operator=(WareHouse&& other) ;// Rule of 5: Move Assignment Operator
        int indexOfId(vector<Order*> orders,int id);
        int indexOfIdVol(vector<Volunteer*> volunteerss,int id);
    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        vector<Customer*> fakeCustomer;
        vector<Volunteer*> fakeVolunteer;
        vector<Order*> fakeOrder; 
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs.
        int orderCounter;
        void parseConfigFile(const string &configFilePath);
        void IdentifyAndPreform(std::string &syntax);
        int getFirstNumber(const std::string& input);
        string extractStringAfterSpaces(const std::string& input, int spaces);
        
        
};


