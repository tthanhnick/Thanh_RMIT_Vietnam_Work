# RMIT University Vietnam
# Course: COSC2429 Introduction to Programming
# Semester: 2021C
# Assignment: 2
# Author: Nguyen Tien Thanh (s3818111)
# Created date: 18/12/2021
# Last modified date: 19/12/2021

def message_decode(str_list, bin_str):
    """
    Concatenate a list of string and sort string in alphabetic order
    :param str_list: list of string
    :param bin_str: number characters string
    :return: sorted string
    """
    # Create variable a list and a string
    new_list = []
    final_message = ""
    # Sort string in a list
    for j in range(0, len(bin_str)):
        # Sort ascending order if digit is 0
        if int(bin_str[j]) % 2 == 0:
            sort_char = sorted(str_list[j], reverse=False)
            new_list.append(sort_char)
        # Sort descending order if digit is 1
        if int(bin_str[j]) % 2 == 1:
            sort_char = sorted(str_list[j], reverse=True)
            new_list.append(sort_char)
    # Turn a list into string
    for i in range(0, len(new_list)):
        final_message += "".join(new_list[i])
    return final_message


# RMIT University Vietnam
# Course: COSC2429 Introduction to Programming
# Semester: 2021C
# Assignment: 2
# Author: Nguyen Tien Thanh (s3818111)
# Created date: 18/12/2021
# Last modified date: 19/12/2021
def tour_sale(sale_dict):
    """
    Returns the total profit of the company in a month, a dictionary containing the commission
    of each salesperson and the salesperson that has the highest profit.
    :param sale_dict: Dictionary contains sale information and salesperson information
    :return: total profit, commission dictionary and best salesperson
    """
    # Create variables with 2 integers ,2 dictionaries, 1 string
    total_profit = 0
    commission_dict = {}
    best_salesperson = {}
    max_persons = ""
    max_value = 0
    # Calculate total profit and commission dictionary
    for key, value in sale_dict.items():
        total_profit += value["luxury"] * 1000 + value["standard"] * 200
        commission_dict[key] = value["luxury"] * 1000 * 0.2 + value["standard"] * 200 * 0.1
    # Find the best salesperson with the highest commission
    for k, v in commission_dict.items():
        # Use if to check the highest commission
        if v > max_value:
            max_value = v
            max_persons = k
    # Substitute key and value to the dictionary
    best_salesperson[max_value] = max_persons
    return total_profit, commission_dict, best_salesperson


# RMIT University Vietnam
# Course: COSC2429 Introduction to Programming
# Semester: 2021C
# Assignment: 2
# Author: Nguyen Tien Thanh (s3818111)
# Created date: 18/12/2021
# Last modified date: 19/12/2021
def shipment_date(product_amount, start_date):
    """
    Days it will take to finish the shipment
    :param product_amount: Number of product
    :param start_date: Date to start working
    :return: Total working date
    """
    # Declare variables 2 integers, product will made and total days to finish
    product = 0
    total_days = 0
    # 0 Sun 1 Mon 2 Tue  3 Wed 4 Thu 5 Fri 6 Sat
    date = [0, 1, 2, 3, 4, 5, 6]
    # A,B,C produce 8, 10, and 12 products each day respectively
    a_product = 8
    b_product = 10
    c_product = 12
    # A off in Sun and Tue, B off in Sun and Wed, C off in Sun Mon and Sat
    a_workday = [1, 3, 4, 5, 6]
    b_workday = [1, 2, 4, 5, 6]
    c_workday = [2, 3, 4, 5]
    # Condition (1)
    condition = True
    # count the number of working date start from ordering to the saturday
    for i in date[start_date:]:
        # Count the number of days
        total_days += 1
        # Count the number of products if workers are available that day
        if i in a_workday:
            product += a_product
        if i in b_workday:
            product += b_product
        if i in c_workday:
            product += c_product
        # Stop if the number of products larger than or equal to ordered products
        if product >= product_amount:
            condition = False
            break
    # count the number of working date start from sunday to the saturday until meets the condition
    while condition:  # Loop until it False
        # Loop through each week to count the number of working date
        for j in date:
            # Count the number of days
            total_days += 1
            # Count the number of products if workers are available that day
            if j in a_workday:
                product += a_product
            if j in b_workday:
                product += b_product
            if j in c_workday:
                product += c_product
            # Stop if the number of products larger than or equal to ordered products
            if product >= product_amount:
                condition = False
                break
    return total_days


# RMIT University Vietnam
# Course: COSC2429 Introduction to Programming
# Semester: 2021C
# Assignment: 2
# Author: Nguyen Tien Thanh (s3818111)
# Created date: 18/12/2021
# Last modified date: 19/12/2021
def encode_str(s):
    """
    Encode a string
    :param s: string
    :return: encoded string
    """
    # Declare variables 2 strings
    encoded_s = ""
    temp_s = ""
    # Change to ASCII code
    for i in s:
        temp_s += str(ord(i))
    # Reverse the string
    temp_s = temp_s[::-1]
    # Encode the string
    for j in range(0, len(temp_s)):
        # Add 1 if it is even
        if j % 2 == 0:
            # 9 will become 0
            if temp_s[j] == "9":
                encoded_s += "0"
            # If it is not 9
            else:
                encoded_s += str(int(temp_s[j]) + 1)
        # The same if it is odd
        else:
            # 9 will become 0
            if temp_s[j] == "9":
                encoded_s += "0"
            # If it is not 9
            else:
                encoded_s += temp_s[j]
    return encoded_s

