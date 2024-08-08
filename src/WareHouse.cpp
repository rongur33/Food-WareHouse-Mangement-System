
#include <string>
#include <vector>
#include <cctype>
using namespace std;
using std::string;
#include <iostream>
#include "../include/Volunteer.h"
#include "../include/Customer.h"
#include "../include/WareHouse.h"
#include "../include/Action.h"
#include <fstream>
#include <sstream>


WareHouse:: WareHouse(const string &configFile): isOpen(false),actionsLog(),volunteers(),
          pendingOrders(),
          inProcessOrders(),
          completedOrders(),
          customers()
           ,fakeCustomer(),fakeVolunteer(),fakeOrder()
          ,customerCounter(0),volunteerCounter(0),orderCounter(0){
            fakeCustomer.push_back(new SoldierCustomer(-1,"fake",-1,-1));
            fakeVolunteer.push_back(new CollectorVolunteer(-1,"fake",-1));
            fakeOrder.push_back(new Order(-1,-1,-1));
    parseConfigFile(configFile);
}
void WareHouse::start(){
    open();
    std::string action;
    std::cout << "Warehouse is open!" << std::endl;
    std::cout << "Please enter a Legal Action" << std::endl;
    std::getline(std::cin,action);
    while(action!="Close")
    {
        IdentifyAndPreform(action);
        std::cout << "Please enter a Legal Action" << std::endl;
        std::getline(std::cin,action);
    }
    Close* closeCommand=new Close();
    closeCommand->act(*this);
    addAction(closeCommand);
}

void WareHouse:: deleteFromProcessOrders(int counter){
   inProcessOrders.erase(inProcessOrders.begin()+counter);
}

void WareHouse:: deleteFromPendingOrders(int counter){
    pendingOrders.erase(pendingOrders.begin()+counter);
}

void WareHouse:: deleteLimitedVolunteer(int counter,int id){
    for(Volunteer* vol:volunteers){
        if(vol->getId()==id){
            delete vol;
            break;
        }
    }
    volunteers.erase(volunteers.begin()+counter);
}

int WareHouse:: indexOfId(vector<Order*> orders,int id){
    int counter=0;
    for(Order* myorders:orders){
        if(myorders->getId()==id){
            return counter;
        }
        counter++;
    }
    return -1;
}

int WareHouse::indexOfIdVol(vector<Volunteer*> volunteerss,int id){
    int counter=0;
    for(Volunteer* myvolunteers:volunteerss){
        if(myvolunteers->getId()==id){
            return counter;
        }
        counter++;
    }
    return -1; 
}


const vector<BaseAction*>& WareHouse::getActions() const{
    return actionsLog;
}
vector<Order*>& WareHouse:: getCompletedOrders(){
    return completedOrders;
}
void WareHouse:: addOrder(Order* order){
    pendingOrders.push_back(order);
    orderCounter++;
}
void WareHouse:: addAction(BaseAction* action){
    actionsLog.push_back(action);
}

int WareHouse:: getVolunteerCounter() const{
    return volunteerCounter;
}

Customer& WareHouse:: getCustomer(int customerId) const{
    for(Customer* cust :customers){
        if(cust->getId()==customerId){
            return *cust ;
        }
    }
return *fakeCustomer.front();
    
}

WareHouse::WareHouse(const WareHouse& other)
    : isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(),fakeCustomer(),fakeVolunteer(),fakeOrder(), customerCounter(other.getCustomerCounter()),volunteerCounter(other.getVolunteerCounter()), orderCounter(other.getOrderCounter())
    
     {
    
    // Deep copy of Customer objects

    for (const auto& customer : other.customers) {
        customers.push_back(customer->clone());
    }

    // Deep copy of Volunteer objects
    for (const auto& volunteer : other.volunteers) {
        volunteers.push_back(volunteer->clone());
    }

    // Deep copy of Order objects in pendingOrders
    for (const auto& order : other.pendingOrders) {
        pendingOrders.push_back(new Order(*order));
    }

    // Deep copy of Order objects in inProcessOrders
    for (const auto& order : other.inProcessOrders) {
        inProcessOrders.push_back(new Order(*order));
    }

    // Deep copy of Order objects in completedOrders
    for (const auto& order : other.completedOrders) {
        completedOrders.push_back(new Order(*order));
    }

    // Deep copy of BaseAction objects
    for (const auto& action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }
    for(const auto& volunteer:other.fakeVolunteer){
        fakeVolunteer.push_back(volunteer->clone());
    }
     for(const auto& order:other.fakeOrder){
        fakeOrder.push_back(new Order(*order));
    }
     for(const auto& customer:other.fakeCustomer){
        fakeCustomer.push_back(customer->clone());
    }
}

 vector<Order*>& WareHouse::getProcessOrders(){
    return inProcessOrders;
}

vector<Order*>& WareHouse:: getPendingOrders(){
    return pendingOrders;
}
const vector<Volunteer*>& WareHouse::getVolunteers() const{
    return volunteers;
}
Volunteer& WareHouse:: getVolunteer(int volunteerId) const{
    for(Volunteer* volunteer :volunteers){
        if(volunteer->getId()==volunteerId){
            return *volunteer ;
        }
    }
    return *fakeVolunteer.front();
}
Order& WareHouse::getOrder(int orderId) const{
    for(Order* _order :pendingOrders){
        if(_order->getId()==orderId){
            return *_order ;
        }
    }
    for(Order* _order :inProcessOrders){
        if(_order->getId()==orderId){
            return *_order ;
        }
    }
    for(Order* _order :completedOrders){
        if(_order->getId()==orderId){
            return *_order ;
        }
    }
    return *fakeOrder.front();
    
}
const vector<Customer*>& WareHouse:: getCustomers() const{
    return customers;
}
int WareHouse:: getOrderCounter() const{
    return orderCounter;
}
void WareHouse::close(){
isOpen=false;
}
void WareHouse::open(){
    isOpen=true;
}

void WareHouse::addCustomer(Customer* _customer){
    customers.push_back(_customer);
    customerCounter++;
} 

int WareHouse::getCustomerCounter() const{
    return customerCounter;
}



void WareHouse:: IdentifyAndPreform(std::string &syntax) {
    int input=getFirstNumber(syntax);
    if(syntax[0]=='s'){
     SimulateStep* steps = new SimulateStep (input);
     steps->act(*this);
     addAction(steps);
    }
    else if (syntax[0]=='v')
    {
       PrintVolunteerStatus* statusPrint = new PrintVolunteerStatus(input);
       statusPrint->act(*this);
      addAction(statusPrint);
    }
    else if (syntax[0]=='l')
    {
         PrintActionsLog* logs=new PrintActionsLog;
         logs->act(*this);
         addAction(logs);
         
    }
    else if (syntax[0]=='b')
    {
        BackupWareHouse* backup=new BackupWareHouse;
        backup->act(*this);
        addAction(backup);
    }
    else if (syntax[0]=='r')
    {
         RestoreWareHouse* restore=new RestoreWareHouse;
         restore->act(*this);
         addAction(restore);
    }
    else if (syntax[0]=='o')
    {
        if(syntax[5]=='S'){
             PrintOrderStatus* _order=new PrintOrderStatus(input);
             _order->act(*this);
             addAction(_order);
             
        }
        else{
             AddOrder* orderAction=new AddOrder(input);
             orderAction->act(*this);
             addAction(orderAction);
        }
    }
    else{
        if (syntax[8]=='S')
        {
             PrintCustomerStatus* costumerStatus= new PrintCustomerStatus(input);
             costumerStatus->act(*this);
             addAction(costumerStatus);
        }
        else{
            string name=extractStringAfterSpaces(syntax,1);
            string role=extractStringAfterSpaces(syntax,2);
            string newDistancestr= extractStringAfterSpaces(syntax,3);
            string maxOrderstr= extractStringAfterSpaces(syntax,4);
            int newDistanceint = std::stoi( newDistancestr);
            int maxOrderint = std::stoi( maxOrderstr);
            AddCustomer* addCus =new AddCustomer(name,role,newDistanceint,maxOrderint);
            addCus->act(*this);
            addAction(addCus);
        }
        
        
    }   
}
int WareHouse:: getFirstNumber(const std::string& input) {
    size_t i = 0;
    std::string numberStr;
    int result;
    // Find the first digit in the string
    while (i < input.length() && !isdigit(input[i])) {
        ++i;
    }

    // Collect the digits to form the number
    while (i < input.length() && isdigit(input[i])) {
        numberStr += input[i];
        ++i;
         // Convert the collected digits to an integer
    result = std::stoi(numberStr);

   
}
return result;
 
}
string WareHouse:: extractStringAfterSpaces(const std::string& input, int spaces) {
    size_t i = 0;

    // Skip the specified number of spaces
    while (i < input.length() && spaces > 0) {
        if (input[i] == ' ') {
            --spaces;
        }
        ++i;
    }

    // Find the start of the next word
    while (i < input.length() && input[i] == ' ') {
        ++i;
    }

    // Collect the letters until the next space
    std::string result;
    while (i < input.length() && input[i] != ' ') {
        result += input[i];
        ++i;
    }

    return result;
}

void WareHouse::parseConfigFile(const string &configFilePath) {
    ifstream configFile(configFilePath);
    string line;
    while (getline(configFile, line)) {
         
        if (line.empty() || line[0] == '#') {
            // Skip empty lines and comments
            continue;
        }

        istringstream iss(line);
        string type;
        iss >> type;

        if (type == "customer") {
            string name, customerType;
            int distance, maxOrders;
            iss >> name >> customerType >> distance >> maxOrders;

            Customer *_newCustomer = nullptr;
            if (customerType == "soldier") {
                _newCustomer = new SoldierCustomer(customerCounter, name, distance, maxOrders);

            } else if (customerType == "civilian") {
                _newCustomer = new CivilianCustomer(customerCounter, name, distance, maxOrders);

            }
            customerCounter++;
            customers.push_back(_newCustomer);
            
        } else if (type == "volunteer") {
            string name, role;
            int cooldown;
            int maxOrders;
            int maxDistance,distancePerStep;
            Volunteer *newVolunteer = nullptr;
            iss >> name >> role;
            if(role== "collector"){
                iss >> cooldown;
                newVolunteer = new CollectorVolunteer(volunteerCounter, name, cooldown);
                volunteerCounter++;
                volunteers.push_back(newVolunteer);
            }else if(role== "limited_collector"){
                iss  >> cooldown >> maxOrders;
                newVolunteer = new LimitedCollectorVolunteer(volunteerCounter, name, cooldown, maxOrders);
                volunteerCounter++;
                volunteers.push_back(newVolunteer);
            }else if(role== "driver"){
                iss >> maxDistance >> distancePerStep; 
                newVolunteer = new DriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep);
                volunteerCounter++;
                volunteers.push_back(newVolunteer);     
            }else{
                iss >> maxDistance >> distancePerStep >> maxOrders;
                newVolunteer = new LimitedDriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep, maxOrders);
                volunteerCounter++;
                volunteers.push_back(newVolunteer);
            }
            
    
}


}
configFile.close();
}


WareHouse::~WareHouse() {
     // Clean up dynamic memory in reverse order of allocation
    for (BaseAction* action : actionsLog) {
        delete action;
    }
    for (Volunteer* volunteer : volunteers) {
        delete volunteer;
    }
    for (Order* order : pendingOrders) {
        delete order;
    }
    for (Order* order : inProcessOrders) {
        delete order;
    }
    for (Order* order : completedOrders) {
        delete order;
    }
    for (Customer* customer : customers) {
        delete customer;
    }
    for(Customer* customer:fakeCustomer){
        delete customer;
    }
    for(Volunteer* volunteer:fakeVolunteer){
        delete volunteer;
    }
    for(Order* order:fakeOrder){
        delete order;
    }
}



// Rule of 5: // Copy assignment operator
WareHouse& WareHouse::operator=(const WareHouse& other) {
    if (this != &other) {  
        WareHouse temp(other);
        std::swap(*this, temp);
    }
        
        return *this;
}

WareHouse::WareHouse(WareHouse&& other) 
    : isOpen(other.isOpen),
      actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)),
      pendingOrders(std::move(other.pendingOrders)),
      inProcessOrders(std::move(other.inProcessOrders)),
      completedOrders(std::move(other.completedOrders)),
      customers(std::move(other.customers)),
      fakeCustomer(std::move(other.fakeCustomer)),
      fakeVolunteer(std::move(other.fakeVolunteer)),
      fakeOrder(std::move(other.fakeOrder)),
      customerCounter(other.customerCounter),
      volunteerCounter(other.volunteerCounter),
      orderCounter(other.orderCounter) 
       {
    // Invalidate the state of the source object
    other.isOpen = false;
    other.customerCounter = 0;
    other.volunteerCounter = 0;
    other.orderCounter = 0;
    other.customers.clear();
    other.volunteers.clear();
    other.pendingOrders.clear();
    other.inProcessOrders.clear();
    other.completedOrders.clear();
    other.actionsLog.clear();
    other.fakeCustomer.clear();
    other.fakeVolunteer.clear();
    other.fakeOrder.clear();
}


// Move Assignment Operator
//Rule of 5: Move assignment operator
WareHouse& WareHouse::operator=(WareHouse&& other)  {
    if (this != &other) {
        std::swap(isOpen, other.isOpen);
        std::swap(customerCounter, other.customerCounter);
        std::swap(volunteerCounter, other.volunteerCounter);
        std::swap(orderCounter, other.orderCounter);
        std::swap(actionsLog, other.actionsLog);
        std::swap(volunteers, other.volunteers);
        std::swap(pendingOrders, other.pendingOrders);
        std::swap(inProcessOrders, other.inProcessOrders);
        std::swap(completedOrders, other.completedOrders);
        std::swap(customers, other.customers);
        std::swap(fakeCustomer, other.fakeCustomer);
        std::swap(fakeVolunteer, other.fakeVolunteer);
        std::swap(fakeOrder, other.fakeOrder);
    }
    return *this;
}




