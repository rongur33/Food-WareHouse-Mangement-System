#include "../include/Volunteer.h"

 Volunteer::Volunteer(int id,const string& name):completedOrderId(NO_ORDER),activeOrderId(NO_ORDER),id(id),name(name){}
 
 int Volunteer::getId() const {
    return id;
}
const string& Volunteer::getName() const {
    return name;
} 
void Volunteer::setCompletedOrderId(int id){
    completedOrderId=id;
}
void Volunteer::resetActiveOrderId(){
    activeOrderId=NO_ORDER;
}
int Volunteer::getActiveOrderId() const {
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const {
    return completedOrderId;
}

bool Volunteer::isBusy() const {
    return activeOrderId != NO_ORDER;
}
// CollectorVolunteer class implementation

CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown)
    : Volunteer(id, name), coolDown(coolDown), timeLeft(coolDown) {}

CollectorVolunteer* CollectorVolunteer::clone() const{
return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step(){
    decreaseCoolDown();
}
int CollectorVolunteer::getCoolDown() const {
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const {
    return timeLeft;
}
bool CollectorVolunteer::decreaseCoolDown() {
    if (timeLeft > 0) {
        timeLeft--;
        return timeLeft == 0;
    }
    return false;
}

bool CollectorVolunteer::hasOrdersLeft() const {
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order& order) const {
    return !isBusy() ;
}

void CollectorVolunteer::acceptOrder(const Order& order) {
    activeOrderId=order.getId();
    timeLeft=coolDown;
}

string CollectorVolunteer::toString() const {
    return " Collector Volunteer " + getName()+". his id is "+std::to_string(getId())+" ."+"His cooldown time is "+std::to_string(getCoolDown())+" and "+std::to_string(getTimeLeft())+" left until the volunteer finishes his current order .";
}

// LimitedCollectorVolunteer class implementation
LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown,int maxOrders)
    : CollectorVolunteer(id, name,coolDown), maxOrders(maxOrders),ordersLeft(maxOrders) {}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const{
return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    if(ordersLeft>=1){return true;}
    else{return false;}
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order& order) const {
    return !isBusy()&&hasOrdersLeft() ;
}

void LimitedCollectorVolunteer::acceptOrder(const Order& order) {
    CollectorVolunteer::acceptOrder(order);
    ordersLeft--;
}


int LimitedCollectorVolunteer::getMaxOrders() const {
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}
string LimitedCollectorVolunteer::toString() const {
    return " Limited Collector Volunteer " + getName()+". his id is " +std::to_string(getId())+" ."+"His cooldown time is "+std::to_string(getCoolDown())+" and "+std::to_string(getTimeLeft())+" left until the volunteer finishes his current order .";
}


// DriverVolunteer class implementation

DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep)
    : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(distancePerStep) {}

DriverVolunteer* DriverVolunteer::clone() const {
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const {
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const {
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const {
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft() {
    if (distanceLeft > 0) {
        distanceLeft -= distancePerStep;
        return distanceLeft <= 0;
    }
    return false;
}

bool DriverVolunteer::hasOrdersLeft() const {
    return true;  
}

bool DriverVolunteer::canTakeOrder(const Order& order) const {
    return !isBusy() && maxDistance>=order.getDistance();
}

void DriverVolunteer::acceptOrder(const Order& order) {
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
}

void DriverVolunteer::step() {
   decreaseDistanceLeft();
}


string DriverVolunteer::toString() const {
    return "Driver Volunteer " + getName()+". his id is " + std::to_string(getId())+" ."+" His maxDistance  is "+std::to_string(getMaxDistance())+" and "+std::to_string(getDistanceLeft())+" left until the volunteer finishes his current order .";
}

// LimitedDriverVolunteer class implementation

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string& name, int maxDistance, int distancePerStep, int maxOrders)
    : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const {
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const {
    return ordersLeft > 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order& order) const {
    return !isBusy() && order.getDistance() <= getMaxDistance() && hasOrdersLeft();
}

void LimitedDriverVolunteer::acceptOrder(const Order& order) {
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;
}

string LimitedDriverVolunteer::toString() const {
    return "Limited Driver Volunteer " + getName();
}








