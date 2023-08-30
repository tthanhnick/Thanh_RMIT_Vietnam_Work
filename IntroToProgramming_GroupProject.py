# shopping application

######################################################################
# cloth item data type
class ClothItem:
    # constructor

    def __init__(self, itemId, name, quantity, color, size, specs, description):
        self.itemId = itemId
        self.name = name
        self.quantity = quantity
        self.color = color
        self.size = size
        self.specs = specs
        self.description = description

    # getters
    def getItemId(self):
        return self.itemId

    def getName(self):
        return self.name

    def getQuantity(self):
        return self.quantity

    def getColor(self):
        return self.color

    def getSize(self):
        return self.size

    def getSpecs(self):
        return self.specs

    def getDescription(self):
        return self.description

    # setters
    def setItemId(self, itemId):
        self.itemId = itemId

    def setName(self, name):
        self.name = name

    def setQuantity(self, quantity):
        self.quantity = quantity

    def setColor(self, color):
        self.color = color

    def setSize(self, size):
        self.size = size

    def setSpecs(self, specs):
        self.specs = specs

    def setDescription(self, description):
        self.description = description

    # print details of item
    def getItemDetail(self):
        return "Item Id: " + str(self.itemId) + ", Item Name: " + self.name + ", Quantity Available: " + str(
            self.quantity) + ", Color: " + self.color + ", Size: " + self.size + ", Specs: " + self.specs + \
               ", Description: " + self.description


# List of items in the store
store_items = {"suit": {"ID": 1, "quantity": 20, "color": 'red', "size": 20,
                        "specs": "both", "description": "formal"
                        },
               "t shirt": {"ID": 2, "quantity": 20, "color": 'blue', "size": 20,
                           "specs": "both", "description": "casual"
                           },
               "blouse": {"ID": 3, "quantity": 20, "color": 'brown', "size": 20,
                          "specs": "both", "description": "formal"
                          },
               "shorts": {"ID": 4, "quantity": 20, "color": 'black', "size": 20,
                          "specs": "both", "description": "casual"
                          },
               "jackets": {"ID": 5, "quantity": 20, "color": 'green', "size": 20,
                           "specs": "both", "description": "formal"
                           },
               "pants": {"ID": 6, "quantity": 20, "color": 'yellow', "size": 20,
                         "specs": "both", "description": "formal"
                         },
               "trousers": {"ID": 7, "quantity": 20, "color": 'white', "size": 20,
                            "specs": "both", "description": "formal"
                            },
               "crop top": {"ID": 8, "quantity": 20, "color": 'violet', "size": 20,
                            "specs": "female", "description": "casual"
                            },
               "jean skirt": {"ID": 9, "quantity": 20, "color": 'gray', "size": 20, "specs": "female",
                              "description": "casual"
                              },
               "evening gown": {"ID": 10, "quantity": 20, "color": 'maroon', "size": 20,
                                "specs": "female", "description": "casual"
                                },
               "ruffle skirt": {"ID": 11, "quantity": 20, "color": 'red', "size": 20,
                                "specs": "female", "description": "casual"
                                },
               "hoodie jacket": {"ID": 12, "quantity": 20, "color": 'blue', "size": 20,
                                 "specs": "both", "description": "casual"
                                 },
               "cargo pants": {"ID": 13, "quantity": 20, "color": 'green', "size": 20,
                               "specs": "both", "description": "casual"
                               },
               "shirt": {"ID": 14, "quantity": 20, "color": 'white', "size": 20,
                         "specs": "both", "description": "formal"
                         },
               "polo shirt": {"ID": 15, "quantity": 20, "color": 'yellow', "size": 20,
                              "specs": "both", "description": "casual"
                              },
               "camp shirt": {"ID": 16, "quantity": 20, "color": 'maroon', "size": 20,
                              "specs": "both", "description": "casual"
                              },
               "swim suit": {"ID": 17, "quantity": 20, "color": 'violet', "size": 20,
                             "specs": "both", "description": "casual"
                             },
               "bikini": {"ID": 18, "quantity": 20, "color": 'black', "size": 20,
                          "specs": "female", "description": "casual"
                          },
               "high heels": {"ID": 19, "quantity": 20, "color": 'gray', "size": 20,
                              "specs": "female", "description": "formal"
                              },
               "sneakers": {"ID": 20, "quantity": 20, "color": 'violet', "size": 20,
                            "specs": "both", "description": "casual"
                            },
               }

# List of customers in the store
store_customers = {"thanh": {"email": "s3818111@rmit.edu.vn", "address": "702 Nguyen Van Linh street, 7, HCMC"
                             },
                   "thang": {"email": "S3927178@rmit.edu.vn", "address": "521 Kim Ma, Ngoc Khanh, Ba Dinh, HN"
                             },
                   "thong": {"email": "S3893648@rmit.edu.vn", "address": "16 Ly Thuong Kiet, Thach Thang, Hai Chau, DN"
                             },
                   }


# 2. List all info of a specific item (quantity, colors, sizes/dims, specs, descriptions, etc.)
def list_all_info_of_a_specific_item(items_store, items_name):
    """
    List info of a specific item
    :param items_store: list of all items store
    :param items_name: input name
    :return:
    """
    # return specific items
    return items_name, items_store[items_name]


# 3. Search item by item name
def search_item_by_item_name(items_store, items_name):
    """
    Search item by item name
    :param items_store: list of all items store
    :param items_name: input name
    :return: name and detail of item found
    """
    # Loop to find the item
    for key, value in items_store.items():
        if key == items_name:
            return key, value


# 4. Search item by item id
def search_item_by_item_id(items_store, items_id):
    """
    Search items by ID
    :param items_id: input ID
    :param items_store: list of all items store
    :return: name and detail of item found
    """
    # Use loop to find item by ID
    for key, value in items_store.items():
        if value["ID"] == items_id:
            return key, value


# 5. List all info of a specific customer (name, email address, shipping address, etc.)
def info_of_a_customer(customers_store, customer_name):
    """
    List info of a customer
    :param customers_store:list of all customers' name details
    :param customer_name: name of a customer
    :return:specific customer info
    """
    # return the specific customer info
    return customer_name, customers_store[customer_name]


# 6. Placing orders (the quantity must be updated when an item is bought)
def placing_order(items_store, items_name, quantity_order):
    """
   Update the quantity when bought an item
    :param items_name: name of an ordered item
   :param items_store: list of all items store
   :param quantity_order: amount of item was bought
   :return:
   """
    # Update the quantity when an item is bought
    for key, value in items_store.items():
        if key == items_name:
            value["quantity"] -= quantity_order
            return key, value


# 7. Search the customer by name
def customer_search_name(customers_store, customer_name):
    """
    Search the customer by name
    :param customers_store: list of all customers in the store
    :param customer_name: input name of the customer
    :return: the name and details of the customer
    """
    # Use loop to find customers by name
    for key, value in customers_store.items():
        if key == customer_name:
            return key, value


# 8. Search the customer by email address
def customer_search_email(customers_store, customer_email):
    """
    Search the customer by email address
    :param customers_store: list of all customers in the store
    :param customer_email: input email of the customer
    :return: the name and details of the customer
    """
    # Use loop to search for customers by email address
    for key, value in customers_store.items():
        if value["email"] == customer_email:
            return key, value


# List of command
print("""
1. List all items 
2. List all info of a specific item (quantity, colors, sizes/dims, specs, descriptions, etc.) 
3. Search item by name 
4. Search item by item id 
5. List all info of a specific customer (name, email address, shipping address, etc.) 
6. Placing orders (the quantity must be updated when an item is bought) 
7. Search customer by name
8. Search customer by email address
""")
command = input("Please enter the number from 1 to 8 you want to use: ")
if command == "1":
    # Q1
    print("List all items", store_items)
elif command == "2":
    # Q2
    search = input("Enter the cloth you want to see info ")
    search = search.casefold()  # lower case
    print("List all info of a specific item ", list_all_info_of_a_specific_item(store_items, search))
elif command == "3":
    # Q3
    search = input("Enter the cloth you want to search ")
    search = search.casefold()  # lower case
    print("Search item by name ", search_item_by_item_name(store_items, search))
elif command == "4":
    # Q4
    search = int(input("Enter the cloth's ID you want to search "))
    print("Search item by ID ", search_item_by_item_id(store_items, search))
elif command == "5":
    # Q5
    search = input("Enter the customer you want to search ")
    search = search.casefold()  # lower case
    print("List all info of a specific customer ", info_of_a_customer(store_customers, search))
elif command == "6":
    # Q6
    search = input("Enter the item you want to buy ")
    search = search.casefold()  # lower case
    search_1 = int(input("Enter the quantity you want to buy "))
    print("Placing orders quantity  ", placing_order(store_items, search, search_1))
elif command == "7":
    # Q7
    search = input("Enter the customer name you want to search ")
    search = search.casefold()  # lower case
    print("Search customer by name ", customer_search_name(store_customers, search))
elif command == "8":
    # Q8
    search = input("Enter the customer email address you want to search ")
    search = search.casefold()  # lower case
    print("Search customer by email account ", customer_search_email(store_customers, search))
else:
    # Return if press the wrong command
    command = input("Please enter the number from 1 to 8 you want to use: ")
