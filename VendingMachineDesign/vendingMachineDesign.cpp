#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
enum class ItemType
{
    Snack,
    Drink,
    Cookie
};

class Item
{
private:
    std::string itemName;
    int itemId;
    ItemType itemType;
    double price;

public:
    Item(int itemId, std::string name, ItemType itemType, double price)
    {
        this->itemId = itemId;
        this->itemName = name;
        this->itemType = itemType;
        this->price = price;
    }
    std::string getItemName() const
    {
        return itemName;
    }
    ItemType getItemType() const
    {
        return itemType;
    }
    double getPrice() const
    {
        return price;
    }
    int getItemId() const
    {
        return itemId;
    }
};

class Shelve
{
private:
    int shelveId;
    ItemType itemType;
    bool isOccupied;

public:
    Shelve(int shelveId, ItemType itemType)
    {
        this->shelveId = shelveId;
        this->itemType = itemType;
        this->isOccupied = false;
    }
    bool isShelveAvailable(ItemType shelveType)
    {
        if (!isOccupied && this->itemType == shelveType)
        {
            return true;
        }
        return false;
    }

    bool vacateShelve()
    {
        if (isOccupied == true)
        {
            isOccupied = false;
            return true;
        }
        return false;
    }

    int getShelveId()
    {
        return shelveId;
    }
    ItemType getShelveType()
    {
        return itemType;
    }

    bool assignShelve()
    {
        if (isShelveAvailable(this->getShelveType()))
        {
            isOccupied = true;
            return true;
        }
        return false;
    }
};

class Inventory
{
private:
    unordered_map<Item *, Shelve *> itemsPlaced;

public:
    Inventory() {}
    Item *isItemAvailable(int itemId)
    {
        for (auto it = itemsPlaced.begin(); it != itemsPlaced.end(); it++)
        {
            if ((*it).first->getItemId() == itemId)
            {
                return (*it).first;
            }
        }
        return nullptr;
    }

    void placeItem(Item *item, Shelve *shelve)
    {
        itemsPlaced[item] = shelve;
    }
    void removeItem(Item *item)
    {
        itemsPlaced.erase(item);
    }
    Shelve *getItemShelve(Item *item)
    {
        return itemsPlaced[item];
    }

    vector<Item*> getListOfItems(){
        vector<Item*> itemList;
        for(auto it=itemsPlaced.begin();it!=itemsPlaced.end();it++){
            itemList.push_back((*it).first);
        }
        return itemList;
    }
};

class VendingMachine
{
private:
    vector<Shelve *> shelves;
    Inventory *inventory;

public:
    VendingMachine()
    {
        inventory = new Inventory();
    }
    bool addItem(Item *item)
    {
        bool itemPlaced = false;
        for (auto it = shelves.begin(); it != shelves.end(); it++)
        {
            if(item->getItemType()==(*it)->getShelveType()){

            
            if ((*it)->isShelveAvailable((*it)->getShelveType()))
            {
                if ((*it)->assignShelve())
                {
                    inventory->placeItem(item, *it);
                    itemPlaced = true;
                    return itemPlaced;
                }
            }}
        }
        return itemPlaced;
    }
    bool removeItem(Item *item)
    {
        bool itemRemoved = false;
        Shelve *shelve = inventory->getItemShelve(item);
        if (shelve->vacateShelve())
        {
            inventory->removeItem(item);
            return true;
        }
        return false;
    }
    Item *dispenseItem(int itemId)
    {
        Item *item = inventory->isItemAvailable(itemId);
        if (item == nullptr)
        {
            return nullptr;
        }
        else
        {
            removeItem(item);
            return item;
        }
    }

    bool addShelve(Shelve *shelve)
    {
        shelves.push_back(shelve);
        return true;
    }
    // bool removeShelve(Shelve *shelve)
    // {
    //     auto it = find(shelves.begin(), shelves.end(), shelve);
    //     if (it != shelves.end())
    //     {
    //         shelves.erase(it);
    //         return true;
    //     }
    //     return false; // Shelve not found
    // }
    ~VendingMachine()
    {
        vector<Item*> items = inventory->getListOfItems();
        for(auto it:items){
            delete it;
        }
        for (int i = 0; i < shelves.size(); i++)
        {
            delete shelves[i];
        }
        delete inventory;
    }
};

class Cart
{
private:
    vector<Item *> itemsInCart;

public:
    Cart() {}
    bool addItemInCart(Item *item)
    {
        itemsInCart.push_back(item);
        return true;
    }
    bool removeItemFromCart(Item *item)
    {
        for (auto it = itemsInCart.begin(); it != itemsInCart.end(); it++)
        {
            if (item->getItemId() == (*it)->getItemId())
            {
                itemsInCart.erase(it);
                return true;
            }
        }
        return false;
    }

    vector<Item *> getItemsInCart()
    {
        return itemsInCart;
    }
};

class Customer
{
private:
    Cart *cart;

public:
    Customer()
    {
        cart = new Cart();
    }
    bool selectItem(Item *item)
    {
        return cart->addItemInCart(item);
    }
    bool unselectItem(Item *item)
    {
        return cart->removeItemFromCart(item);
    }
    Cart *getCart()
    {
        return cart;
    }
    vector<Item *> getItemsFromCart()
    {
        return cart->getItemsInCart();
    }
    ~Customer(){
        delete cart;
    }
};

class Checkout
{
    Cart *cart;
    int totalAmount;

public:
    Checkout(Cart *cart)
    {
        this->cart = cart;
        this->totalAmount = 0;
    }
    double calculateTotalAmount()
    {
        vector<Item *> itemsInCart = cart->getItemsInCart();
        if (!itemsInCart.empty())
        {
            for (auto it = itemsInCart.begin(); it != itemsInCart.end(); it++)
            {
                totalAmount += (*it)->getPrice();
            }
        }
        return totalAmount;
    }
    ~Checkout(){
        delete cart;
    }
};

class Payment
{
private:
    bool success;

public:
    Payment()
    {
        this->success = false;
    }
    bool processPayment(double amount)
    {
        return true;
    }
};

int main()
{
    VendingMachine *machine = new VendingMachine();
    cout << "here" << endl;
    machine->addShelve(new Shelve(1, ItemType::Drink));
    machine->addShelve(new Shelve(2, ItemType::Snack));
    machine->addShelve(new Shelve(3, ItemType::Cookie));
    cout << "here2" << endl;
    // Inventory *inv = new Inventory();
    cout << "here3" << endl;

    Item *item1 = new Item(1, "RedbullDrink", ItemType::Drink, 120);
    Item *item2 = new Item(2, "Kurkure", ItemType::Snack, 15);
    Item *item3 = new Item(3, "50-50", ItemType::Cookie, 25);
    cout << "here4" << endl;

    machine->addItem(item1);
    machine->addItem(item2);
    machine->addItem(item3);
    cout << "here5" << endl;

    Customer *c1 = new Customer();
    cout << "6" << endl;

    c1->selectItem(item1);
    cout << "8" << endl;

    c1->selectItem(item3);
    cout << "here7" << endl;

    Checkout *c = new Checkout(c1->getCart());
    Payment pay;
    bool amountPaid = pay.processPayment(c->calculateTotalAmount());
    if (amountPaid)
    {
        vector<Item *> itemsToDispense = c1->getItemsFromCart();
        for (auto it : itemsToDispense)
        {
            machine->dispenseItem((*it).getItemId());
        }
    }
    delete c;
    delete machine;
}
