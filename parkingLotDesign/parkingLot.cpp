#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

enum class VehicleType
{
    Bike,
    Car,
    Truck
};

enum PaymentMode
{
    Cash,
    Card,
    UPI
};

class Vehicle
{
private:
    string number;
    VehicleType type;

public:
    Vehicle(string number, VehicleType type)
    {
        this->number = number;
        this->type = type;
    }
    VehicleType getVehicleType() const
    {
        return type;
    }
    string getVehicleNumber() const
    {
        return number;
    }
};

class ParkingSpot
{
private:
    int spotId;
    VehicleType allowedVehType;
    Vehicle *vehicle;

public:
    ParkingSpot(int spotId, VehicleType type)
    {
        this->spotId = spotId;
        this->allowedVehType = type;
        this->vehicle = nullptr;
    }

    bool isSpotAvailable(VehicleType type)
    {
        return vehicle == nullptr && type == allowedVehType;
    }

    bool assignSpot(Vehicle &vehicle)
    {
        if (isSpotAvailable(vehicle.getVehicleType()))
        {
            this->vehicle = &vehicle;
            return true;
        }
        return false;
    }
    bool freeSpot()
    {
        if (vehicle != nullptr)
        {
            vehicle = nullptr;
            return true;
        }
        return false;
    }

    int getSpotId()
    {
        return spotId;
    }
};

class Level
{
private:
    int levelNumber;
    vector<ParkingSpot *> spots;

public:
    Level(int levelNumber)
    {
        this->levelNumber = levelNumber;
    }
    void addParkingSpot(ParkingSpot *spot)
    {
        spots.push_back(spot);
    }
    void removeParkingSpot(ParkingSpot *spot)
    {
        for (auto it = spots.begin(); it != spots.end(); ++it)
        {
            if (*it == spot)
            {
                spots.erase(it);
                break;
            }
        }
    }

    ParkingSpot *findFreeSpot(VehicleType type)
    {
        for (auto spot : spots)
        {
            if (spot->isSpotAvailable(type))
            {
                return spot;
            }
        }
        return nullptr;
    }
    int getLevelNumber()
    {
        return levelNumber;
    }
    ~Level()
    {
        for (auto spot : spots)
        {
            delete spot;
        }
    }
};

class Ticket
{
private:
    int ticketId;
    const time_t entryTime;
    Vehicle *vehicle;
    ParkingSpot *spot;
    static int nextTicketId;
    time_t exitTime;

public:
    Ticket(Vehicle *vehicle, ParkingSpot *spot): entryTime(time(nullptr))
    {
        this->ticketId = nextTicketId++;
        this->vehicle = vehicle;
        this->spot = spot;
    }
    ParkingSpot *getParkingSpot()
    {
        return spot;
    }
    time_t getEntryTime()
    {
        return entryTime;
    }

    void setExitTime(time_t exitTime)
    {
        this->exitTime = exitTime;
    }

    time_t getExitTime()
    {
        return exitTime;
    }

    Vehicle *getVehicle()
    {
        return vehicle;
    }
};

int Ticket::nextTicketId = 1000;

class pricingStrategy
{
public:
    virtual double calculateParkingFee(time_t entryTime, time_t exitTime) = 0;
    virtual ~pricingStrategy() {}
};

class HourlyPricingStrategy : public pricingStrategy
{
private:
    double ratePerHour;

public:
    HourlyPricingStrategy(double ratePerHour)
    {
        this->ratePerHour = ratePerHour;
    }
    double calculateParkingFee(time_t entryTime, time_t exitTime) override
    {
        double duration = difftime(exitTime, entryTime) / 3600; // duration in hours
        return duration * ratePerHour;
    }
};

class Payment
{
private:
    bool success;

public:
    Payment()
    {
        success = false;
    }
    bool payment(double amount, PaymentMode mode)
    {
        // Simulate payment processing
        cout << "Processing payment of amount: " << amount << " using mode: " << mode << endl;
        success = true; // Assume payment is always successful for this example
        return success;
    }
};

class Bill
{
private:
    pricingStrategy *strategy;

public:
    Bill(pricingStrategy *strategy)
    {
        this->strategy = strategy;
    }

    double generateBill(Ticket *ticket)
    {
        ticket->setExitTime(time(nullptr));
        return strategy->calculateParkingFee(ticket->getEntryTime(), ticket->getExitTime());
    }

    bool processPayment(double amount, PaymentMode mode)
    {
        Payment payment;
        return payment.payment(amount, mode);
    }
};

class ParkingLot
{
private:
    vector<Level *> levels;

public:
    ParkingLot() {}
    Ticket *parkVehicle(Vehicle &vehicle)
    {
        bool parked = false;
        for (auto level : levels)
        {
            ParkingSpot *spot = level->findFreeSpot(vehicle.getVehicleType());
            if (spot != nullptr)
            {
                spot->assignSpot(vehicle);
                parked = true;
                return new Ticket(&vehicle, spot);
            }
        }
        return nullptr;
    }

    void addLevel(Level *level)
    {
        levels.push_back(level);
    }
    void removeLevel(Level *level)
    {
        for (auto it = levels.begin(); it != levels.end(); ++it)
        {
            if (*it == level)
            {
                levels.erase(it);
                break;
            }
        }
    }

    bool unparkVehicle(Ticket *ticket)
    {
        ParkingSpot *spot = ticket->getParkingSpot();
        // Bill* bill = new Bill(ticket,50);
        // cout<<"Parking Duration (minutes): "<<bill->getDuration()<<endl;
        // cout<<"Total Amount to be paid: "<<bill->getAmount()<<endl;
        if (spot->freeSpot())
        {
            return true;
        }
        else
            return false;
    }

    ~ParkingLot()
    {
        for (auto level : levels)
        {
            delete level;
        }
    }
};

int main()
{
    Level *level1 = new Level(1);
    level1->addParkingSpot(new ParkingSpot(101, VehicleType::Car));
    level1->addParkingSpot(new ParkingSpot(102, VehicleType::Bike));
    ParkingLot *parkingLot = new ParkingLot();
    parkingLot->addLevel(level1);
    Vehicle vehicle1("KA-01-1234", VehicleType::Car);
    Ticket *ticket = parkingLot->parkVehicle(vehicle1);
    if (ticket != nullptr)
    {
        cout << "Vehicle parked successfully. Ticket ID: " << ticket->getParkingSpot()->getSpotId() << endl;
    }
    else
    {
        cout << "No available spot for the vehicle." << endl;
    }
    int k;
    cout << "Enter ket:" << endl;
    cin >> k;
    pricingStrategy* strategy = new HourlyPricingStrategy(50); // 50 currency units per hour
    Bill bill(strategy); // 50 currency units per hour
    double amount = bill.generateBill(ticket);
    bool paid = bill.processPayment(amount, PaymentMode::UPI);
    if (paid)
    {
        cout<<"Payment successful. Unparking vehicle..."<<endl;
        bool unparked = parkingLot->unparkVehicle(ticket);
        if (unparked)
        {
            cout << "Vehicle successfully unparked." << endl;
        }
        else
        {
            cout << "Error in unparking vehicle." << endl;
        }
    }
    delete ticket;
    delete parkingLot;
}
