#include "../include/Action.h"
#include <iostream>
#include "../include/Volunteer.h"
BaseAction:: BaseAction():errorMsg(""),status(ActionStatus::COMPLETED){
}
ActionStatus BaseAction:: getStatus() const{
     return status;
}
void BaseAction:: complete(){
status=ActionStatus::COMPLETED;
}
void BaseAction:: error(string errorMsg){
 this -> errorMsg=errorMsg;
 std::cout << getErrorMsg() << std::endl;
 status=ActionStatus::ERROR;
}
string BaseAction:: getErrorMsg() const{
    return errorMsg;
}

string BaseAction:: enumToString(ActionStatus status) const{
    switch (status) {
        case ActionStatus::COMPLETED:
            return "completed";
        case ActionStatus::ERROR:
            return "error";
        default:
            return "unknown"; 
    }
}

SimulateStep::SimulateStep(int numOfSteps):numOfSteps(numOfSteps){}

void SimulateStep::act(WareHouse &wareHouse){
    //step 1
    for(int i = 1; i <= numOfSteps; i++){
        vector<int> toDeleteID(0);
          for(Order* myorder:wareHouse.getPendingOrders()){
            vector<Volunteer*> myVolunteers = wareHouse.getVolunteers();            
            if (myorder->getStatus() == OrderStatus::PENDING) {
                for(Volunteer* myVolenteer:wareHouse.getVolunteers()) {
                    CollectorVolunteer* collectorVolunteer = dynamic_cast<CollectorVolunteer*>(myVolenteer);
                    if (collectorVolunteer != nullptr) {
                        if (collectorVolunteer->canTakeOrder(*myorder)) {
                            myorder->setStatus(OrderStatus::COLLECTING);
                            myorder->setCollectorId(myVolenteer->getId());
                            wareHouse.getProcessOrders().push_back(myorder);
                            myVolenteer->acceptOrder(*myorder);
                            toDeleteID.push_back(myorder->getId());
                            break;
                        }
                    }  
                }
            } else {
                for(Volunteer* myVolenteer:wareHouse.getVolunteers()) {
                    DriverVolunteer* driverVolunteer = dynamic_cast<DriverVolunteer*>(myVolenteer);
                    if (driverVolunteer != nullptr) {
                        if (driverVolunteer->canTakeOrder(*myorder)) {
                            myorder->setStatus(OrderStatus::DELIVERING);
                            myorder->setDriverId(myVolenteer->getId());
                            wareHouse.getProcessOrders().push_back(myorder);
                            myVolenteer->acceptOrder(*myorder);
                            toDeleteID.push_back(myorder->getId());
                            break;
                        }
                    }
                }
            }
        }
        for(int id:toDeleteID){
            int counter=wareHouse.indexOfId(wareHouse.getPendingOrders(),id);
            wareHouse.deleteFromPendingOrders(counter);
        }
        //step 2
        for(Volunteer* myVolenteer:wareHouse.getVolunteers()){ 
            myVolenteer->step();
        }
        //step 3
        vector<int> toDeleteId2(0); 
        for(Volunteer* myVolenteer:wareHouse.getVolunteers()){
            if(myVolenteer->isBusy())
            {
                if (CollectorVolunteer* collectorVolunteer = dynamic_cast<CollectorVolunteer*>(myVolenteer)){
                    if(collectorVolunteer->getTimeLeft()==0){
                        for(Order* thisOrder:wareHouse.getProcessOrders())
                        {
                            if(thisOrder->getId()==myVolenteer->getActiveOrderId()){
                                wareHouse.getPendingOrders().push_back(thisOrder);
                                thisOrder->setStatus(OrderStatus::COLLECTING);
                                myVolenteer->setCompletedOrderId(myVolenteer->getActiveOrderId());
                                myVolenteer->resetActiveOrderId();
                                toDeleteId2.push_back(thisOrder->getId());
                                break;
                            }
                        }
                    }
                }
                else if(DriverVolunteer* driverVolunteer = dynamic_cast<DriverVolunteer*>(myVolenteer)){
                    if(driverVolunteer->getDistanceLeft()<=0){
                        for(Order* thisOrder:wareHouse.getProcessOrders())
                        {
                            if(thisOrder->getId()==myVolenteer->getActiveOrderId()){
                                wareHouse.getCompletedOrders().push_back(thisOrder);
                                thisOrder->setStatus(OrderStatus::COMPLETED);
                                myVolenteer->setCompletedOrderId(myVolenteer->getActiveOrderId());
                                myVolenteer->resetActiveOrderId();
                                toDeleteId2.push_back(thisOrder->getId());
                                break;
                            }
                        }
                    }
               }
            }
           
        }
        for(int id:toDeleteId2){
            int counter=wareHouse.indexOfId(wareHouse.getProcessOrders(),id);
            wareHouse.deleteFromProcessOrders(counter);
        }
        //step 4
        vector<int> toDeleteId3(0);
        for(Volunteer* myVolunteer:wareHouse.getVolunteers()){
            if(LimitedCollectorVolunteer* limitedCollector = dynamic_cast<LimitedCollectorVolunteer*>(myVolunteer)){
                if(limitedCollector->getNumOrdersLeft()<=0&&(myVolunteer->isBusy()==false)){
                    toDeleteId3.push_back(myVolunteer->getId());
                }
            }
            else if(LimitedDriverVolunteer* limitedDriver = dynamic_cast<LimitedDriverVolunteer*>(myVolunteer)){
                if(limitedDriver->getNumOrdersLeft()<=0&&(myVolunteer->isBusy()==false)){
                    toDeleteId3.push_back(myVolunteer->getId());
                }
            }
        }
        for(int id:toDeleteId3){
            int counter=wareHouse.indexOfIdVol(wareHouse.getVolunteers(),id);
            wareHouse.deleteLimitedVolunteer(counter,id);
        }
    }
    complete();
}

SimulateStep* SimulateStep::clone() const{
    return new SimulateStep(*this);
}

string SimulateStep:: toString() const{
    return "simulateStep "+std::to_string(numOfSteps)+" "+enumToString(getStatus());
}



AddOrder::AddOrder(int id): customerId(id){
}

void AddOrder::act(WareHouse &wareHouse){
    bool isFound=false;
    for(Customer* myCustomer:wareHouse.getCustomers()){
        if(myCustomer->getId()==customerId){
            isFound=true;
            if(myCustomer->canMakeOrder()==false){
                error("Cannot place this order");
                break;
            }
            else{
                Order* newOrder=new Order(wareHouse.getOrderCounter(),customerId,myCustomer->getCustomerDistance());
                myCustomer->addOrder(wareHouse.getOrderCounter());
                newOrder->setStatus(OrderStatus::PENDING);
                wareHouse.addOrder(newOrder);
                complete();
                break;
            }
        }
    }
    if(isFound==false){
        error("Cannot place this order");
    }
}
AddOrder* AddOrder::clone() const{
    return new AddOrder(*this);
}
string AddOrder:: toString() const{
    return "Order "+std::to_string(customerId)+" "+enumToString(getStatus());
}

AddCustomer::AddCustomer(const string& customerName, const string& customerType, int distance, int maxOrders):customerName(customerName),customerType((customerType=="civilian")?CustomerType::Civilian:CustomerType::Soldier),distance(distance),maxOrders(maxOrders) {
}
void AddCustomer:: act(WareHouse &wareHouse){
    if(customerType==CustomerType::Soldier){
        SoldierCustomer* newSoldier=new SoldierCustomer(wareHouse.getCustomerCounter(),customerName,distance,maxOrders);
        wareHouse.addCustomer(newSoldier); 
    }
    else{
        CivilianCustomer* newCivilian=new CivilianCustomer(wareHouse.getCustomerCounter(),customerName,distance,maxOrders);
        wareHouse.addCustomer(newCivilian); 
    }
    complete();
}
AddCustomer* AddCustomer:: clone() const{
    return new AddCustomer(*this);
}
string AddCustomer:: toString() const{
    return "Customer "+customerName+" "+enumToString(getStatus())+" "+std::to_string(distance)+" "+std::to_string(maxOrders)+" "+enumToString(getStatus());  
}

PrintOrderStatus::PrintOrderStatus(int id):orderId(id){}

void PrintOrderStatus:: act(WareHouse &wareHouse){
    if(orderId>wareHouse.getOrderCounter()-1){
        error("ERROR: order doesn't exist");
    }
    else{
        Order& myOrder = wareHouse.getOrder(orderId);
        std::cout << myOrder.toString() << std::endl;
        complete();
 }
}

PrintOrderStatus* PrintOrderStatus::clone() const{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus:: toString() const{
    return "orderStatus "+std::to_string(orderId)+" : "+enumToString(getStatus());
}

PrintCustomerStatus:: PrintCustomerStatus(int customerId):customerId(customerId){}

void PrintCustomerStatus:: act(WareHouse &wareHouse){
    if(customerId>wareHouse.getCustomerCounter()-1){
        error("ERROR: Customer doesn't exist");
    }
    else{
        Customer& currCustomer= wareHouse.getCustomer(customerId);
        std::cout << "CustomerID: "+ std::to_string(currCustomer.getId()) << std::endl;
        vector<int> myOrdersIds=currCustomer.getOrdersIds();
        for(vector<int>::iterator it = myOrdersIds.begin(); it != myOrdersIds.end(); ++it){
            std::cout << "OrderID: " << *it << std::endl;
            Order& myOrder = wareHouse.getOrder(*it);
            std::cout << "OrderStatus: " + myOrder.enumToString(myOrder.getStatus()) << std::endl;
        }
        std::cout << "NumOrdersLeft: " +std::to_string(currCustomer.getMaxOrders()-currCustomer.getNumOrders())  << std::endl;
        complete();
    }
}

PrintCustomerStatus* PrintCustomerStatus::clone() const{
    return new PrintCustomerStatus(*this);
}
string PrintCustomerStatus:: toString() const{
    return "customerStatus "+std::to_string(customerId)+" : "+enumToString(getStatus()); 
}

PrintVolunteerStatus:: PrintVolunteerStatus(int id):VolunteerId(id){}

void PrintVolunteerStatus:: act(WareHouse &wareHouse){
    Volunteer& myVolunteer=wareHouse.getVolunteer(VolunteerId);
    if(myVolunteer.getId()==-1){
        error("Error: Volunteer doesn't exist”");
    }
    else{
        std::cout << "VolunteerID: "+ std::to_string(myVolunteer.getId()) << std::endl;
        if(myVolunteer.isBusy()){
             std::cout << "isBusy: TRUE" << std::endl;
        }else{
            std::cout << "isBusy: FALSE" << std::endl;
        }
        if(myVolunteer.getActiveOrderId()==NO_ORDER){
            std::cout << "OrderID: None" << std::endl;
        }
        else{
            std::cout << "OrderID: "+std::to_string( myVolunteer.getActiveOrderId()) << std::endl;
        }
        if (CollectorVolunteer* collectorVolunteer = dynamic_cast<CollectorVolunteer*>(&myVolunteer)) {
                if(collectorVolunteer->isBusy()==true){
                    std::cout << "timeLeft: "+std::to_string(collectorVolunteer->getTimeLeft()) << std::endl;
                }
                else{
                    std::cout << "Time left: None" << std::endl;
                }
        }
        else if (DriverVolunteer* driverVolunteer = dynamic_cast<DriverVolunteer*>(&myVolunteer)) {
            if(driverVolunteer->isBusy()==true){
                std::cout << "distance left: "+std::to_string( driverVolunteer->getDistanceLeft()) << std::endl;
            }
            else{
                std::cout << "Distance left: None" << std::endl;
            }
        }
        if(LimitedCollectorVolunteer* limitedCollectorVolunteer = dynamic_cast<LimitedCollectorVolunteer*>(&myVolunteer)){
            std::cout << "Orders Left: "+std::to_string(limitedCollectorVolunteer->getNumOrdersLeft()) << std::endl;
        }
        else if(LimitedDriverVolunteer* limitedDriverVolunteer = dynamic_cast<LimitedDriverVolunteer*>(&myVolunteer)){
             std::cout << "Orders Left: "+std::to_string( limitedDriverVolunteer->getNumOrdersLeft()) << std::endl;
        }
        else{
            std::cout << "Orders left: no limit" << std::endl;
        }
        complete();
    }  
}

PrintVolunteerStatus * PrintVolunteerStatus ::clone() const{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus:: toString() const{
    return "volunteerStatus"+std::to_string(VolunteerId)+" : "+enumToString(getStatus()); 
}

PrintActionsLog:: PrintActionsLog(){};

void PrintActionsLog:: act(WareHouse &wareHouse){
    vector<BaseAction*> myActions=wareHouse.getActions();
    for(vector<BaseAction*>::iterator it = myActions.begin(); it != myActions.end(); ++it){
        std::cout << (*it)->toString() << std::endl;
    }
    complete();
}

PrintActionsLog * PrintActionsLog ::clone() const{
    return new PrintActionsLog(*this);
}

string PrintActionsLog:: toString() const{
    return "log: "+enumToString(getStatus()); 
}

Close:: Close(){};

void Close:: act(WareHouse &wareHouse){
    for(Order* myOrder:wareHouse.getPendingOrders()){
        std::cout << "OrderID: "+std::to_string( myOrder->getId())+",CustomerID: "+std::to_string(myOrder->getCustomerId())+", Status: "+myOrder->enumToString(myOrder->getStatus()) << std::endl;
    }
    for(Order* myOrder:wareHouse.getProcessOrders()){
        std::cout << "OrderID: "+std::to_string( myOrder->getId())+",CustomerID: "+std::to_string(myOrder->getCustomerId())+", Status: "+myOrder->enumToString(myOrder->getStatus()) << std::endl;
    }
    for(Order* myOrder:wareHouse.getCompletedOrders()){
        std::cout << "OrderID: "+std::to_string( myOrder->getId())+",CustomerID: "+std::to_string(myOrder->getCustomerId())+", Status: "+myOrder->enumToString(myOrder->getStatus()) << std::endl;
    }
    wareHouse.close();
    complete();
}

Close* Close::clone() const{
    return new Close(*this);
}

string Close:: toString() const{
    return "Close: "+enumToString(getStatus()); 
}

BackupWareHouse::BackupWareHouse(){}

void BackupWareHouse::act(WareHouse &wareHouse)
{
   
    if (backup != nullptr)
    {
        delete (backup);
    }
    backup = new WareHouse(wareHouse);
    complete();
}

string BackupWareHouse::toString() const
{
    return "backupWareHouse: "+enumToString(getStatus());
}

BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}

RestoreWareHouse::RestoreWareHouse(){}

void RestoreWareHouse::act(WareHouse &wareHouse)
{
    if (backup == nullptr)
    {
        error("Backup does not exist");
    }
    else
    {
        wareHouse = *backup;
        complete();
    }
}

string RestoreWareHouse::toString() const
{
    return "restoreWareHouse: "+enumToString(getStatus());
}

RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}










