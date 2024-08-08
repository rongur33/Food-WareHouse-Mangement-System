#include "../include/Order.h"


Order::Order (int id, int customerId, int distance):
id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER){};

int Order::getId() const{
    return id;
}

int Order:: getCustomerId() const{
    return customerId;
}

int Order:: getDistance() const{
    return distance;
}

string Order:: enumToString(OrderStatus status) const{
    switch (status) {
        case OrderStatus::PENDING:
            return "pending";
        case OrderStatus::COLLECTING:
            return "collecting";
        case OrderStatus::DELIVERING:
            return "delivering";
        case OrderStatus::COMPLETED:
            return "completed";
        default:
            return "None";
    }
}

void Order::setStatus(OrderStatus status){
    this -> status = status ;

}

void Order:: setCollectorId(int collectorId){
    this -> collectorId = collectorId;
}

void Order:: setDriverId(int driverId){
    this -> driverId = driverId;
}

int Order::getCollectorId() const{
    return collectorId;
}

int Order:: getDriverId() const{
    return driverId;
}

OrderStatus Order:: getStatus() const{
    return status;
}

const string Order:: toString() const{
    if(driverId==-1&&collectorId==-1){
        return "OrderId: " +std::to_string(getId())+ "\n"+"OrderStatus: "+enumToString(status)+"\n" +"CustomerID: " + std::to_string(getCustomerId())+ "\n"+"Collector ID: None"+"\n"+"Driver ID: None";
    }
    else if(collectorId!=-1&&driverId==-1){
         return "OrderId: " +std::to_string(getId())+ "\n"+"OrderStatus: "+ enumToString(status)+"\n" +"CustomerID: " + std::to_string(getCustomerId())+ "\n"+"Collector ID: "+std::to_string(getCollectorId())+ "\n"+"Driver ID: None";
    }
    else{
        return "OrderId: " +std::to_string(getId())+ "\n"+"OrderStatus: "+ enumToString(status)+"\n" +"CustomerID: " + std::to_string(getCustomerId())+ "\n"+ +"Collector ID: " + std::to_string(getCollectorId()) +"\n" "Driver ID: " + std::to_string(getDriverId());
    }  
}
