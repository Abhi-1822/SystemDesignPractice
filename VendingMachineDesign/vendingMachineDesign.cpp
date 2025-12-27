#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
using namespace std;
enum class ItemType{
    Snack,
    Drink,
    Cookie
};


class Item {
    private:
        std:: string itemName;
        int itemId;
        ItemType itemType;
        double price;
    public:
        Item(std::string name, ItemType itemType, double price){
            this->itemName = name;
            this->itemType = itemType;
            this->price = price;
        }
        std::string getItemName(){
            return itemName;
        }
        ItemType getItemType(){
            return itemType;
        }
        double getPrice(){
            return price;
        }
        int getItemId(){
            return itemId;
        }
};

class Shelve{
    private:
        int shelveId;
        ItemType itemType;
        bool isOccupied;
    public:
        Shelve(int shelveId, ItemType itemType){
            this->shelveId = shelveId;
            this->itemType  = itemType;
            this->isOccupied = false;
        }
        bool isShelveAvailable(ItemType shelveType){
            if(isOccupied && this->itemType == shelveType) return true;
            return false;
        }

        bool vacateShelve(){
            if(isOccupied==true){
                isOccupied = false;
                return true;
            }
            return false;
        }

        int getShelveId(){
            return shelveId;
        }
        ItemType getShelveType(){
            return itemType;
        }

        bool assignShelve(){
            if(isShelveAvailable(this->getShelveType())){
                return true;
            }
            return false;
        }
};

class Inventory{
    private:
        vector<Shelve*> shelves;
        unordered_map<Item*,Shelve*> itemsPlaced;
    public:
        Inventory(){}
        bool addItem(Item *item){
            bool itemPlaced = false;
            for(auto it=shelves.begin();it!=shelves.end();it++){
                if((*it)->isShelveAvailable((*it)->getShelveType())){
                    if((*it)->assignShelve()){
                        itemsPlaced[item] = *it;
                        itemPlaced = true;
                        return itemPlaced;
                    }
                }
            }
            return itemPlaced;

        }
        bool removeItem(Item *item){
            bool itemRemoved=false;
            Shelve* shelve = itemsPlaced[item];
            if(shelve->vacateShelve()){
                itemsPlaced.erase(item);
                return true;
            }
            return false;
        }

        Item* isItemAvailable(int itemId){
            for(auto it=itemsPlaced.begin();it!=itemsPlaced.end();it++){
                if((*it).first->getItemId()==itemId){
                    return (*it).first;
                }
            }
            return nullptr;
        }
};

class VendingMachine{
    private:
        Inventory *inventory;
    public:
        VendingMachine(){}
        Item* dispenseItem(int itemId){
            Item* item = inventory->isItemAvailable(itemId);
            if(item==nullptr){
                return nullptr;
            }
            else{
                inventory->removeItem(item);
                return item;
            }
        }
};

class Cart{
    private:
        vector<Item*> itemsInCart;
    public:
        Cart(){}
        bool addItemInCart(Item* item){
            itemsInCart.push_back(item);
            return true;
        }
        bool removeItemFromCart(Item* item){
            for(auto it=itemsInCart.begin();it!=itemsInCart.end();it++){
                if(item->getItemId()==(*it)->getItemId()){
                    itemsInCart.erase(it);
                    return true;
                }
            }
            return false;
        }

        vector<Item*> getItemsInCart(){
            return itemsInCart;
        }
};

class Customer{
    private:
        Cart* cart;
    public:
        Customer(){}
        bool selectItem(Item* item){
            return cart->addItemInCart(item);
        }
        bool unselectItem(Item* item){
            return cart->removeItemFromCart(item);
        }
};

class Checkout{
    Cart * cart;
    int totalAmount;
    public:
        Checkout(Cart* cart){
            this->cart = cart;
            this->totalAmount = 0;
        }
        double calculateTotalAmount(){
            vector<Item*> itemsInCart = cart->getItemsInCart();
            if(!itemsInCart.empty()){
                for(auto it=itemsInCart.begin();it!=itemsInCart.end();it++){
                    totalAmount+=(*it)->getPrice();
                }
            }
            return totalAmount;
        }
};

class Payment{
    private:
        bool success;
    public:
        Payment(){
            this->success=false;
        }
        bool processPayment(double amount){
            return true;
        }
};

int main(){
    
    Inventory* inv = new Inventory();
    inv->addItem(new Item("RedbullDrink", ItemType::Drink, 120));
    inv->addItem(new Item("Kurkure", ItemType::Snack, 15));
    inv->addItem(new Item("50-50", ItemType::Cookie, 25));
}













