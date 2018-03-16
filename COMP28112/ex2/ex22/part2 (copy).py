# Import the required packages
from time import time as getTime
from getpass import getpass as raw_pass
import time
import requests
import re

# The hotel's server url_band.
url_band = "http://jewel.cs.man.ac.uk:3010/queue/enqueue"
url_hotel = "http://jewel.cs.man.ac.uk:3020/queue/enqueue"

# Default headers for different requests method.
headers = {"Content-Type": "application/xml", "Accept": "application/xml"}

# Dictionary to interprete the server's respons code.
server_responses = {200: "Message sucessfully sent.",
                    401: "Invalid username/password.",
                    403: "Given slot ID doesn't exist.",
                    404: "Error, request has not been processed.",
                    405: "Invalid request",
                    409: "Input slot ID is taken",
                    503: "Service is unavailable at the moment."}

global current_ID
global wait_time; wait_time = 1
global username, password, request_result

# To remember info
username = "mn2114"
password = "bMr879"

# Function to request a reservation for the client.
def reservation_request(url, slot_ID):
    # Get current time as the request ID, as it's always unique.
    current_ID = int(getTime() * 100)

    # Dictionary to store data that will be convert to XML string
    xmlData = {"request_ID": current_ID, "username": username,
               "password": password, "slot_ID": slot_ID}

    xmlTemplate = """<reserve>
<request_id>&&request_ID&&</request_id>
<username>&&username&&</username>
<password>&&password&&</password>
<slot_id>&&slot_ID&&</slot_id>
</reserve>"""

    for key in xmlData:
        oldValue = "&&" + str(key) + "&&"
        xmlTemplate = xmlTemplate.replace(oldValue, str(xmlData[key]))

    # Sending request
    request_result = requests.put(url, data=xmlTemplate, headers=headers)
    time.sleep(wait_time)

    # Checking the result status.
    if request_result.status_code == 200:
        print "Reservation request was sent sucessfully"
        print "Request ID:", current_ID
        print "Slot ID:", slot_ID

        replyURL = (request_result.content + "?username=" + username
                    + "&password=" + password)
        replyURL = re.sub('<[^>]*>', '', replyURL)

        print replyURL

        server_result = requests.get(replyURL)

        # Check user login details.
        if server_result.status_code == 401:
            print "Error: 401", server_responses[401]
            return

        if server_result.status_code != 200:
            print "There was a problem processing the request"
            print "Processing again..."

        # While loop to get the correct out come of a successful request.
        while server_result.status_code != 200:
            server_result = requests.get(replyURL)
            time.sleep(wait_time)

        server_result_simple = re.sub('<[^>]*>', '', server_result.text)

        # Print out the result of the request.
        result_code = re.findall("<code>(.*?)</code>", server_result.text)

        if int(result_code[0]) != 200:
            print "Error", result_code[0], ":", server_responses[int(result_code[0])]
            return -1
        else:
            return 0
        print "========================================================"
    else:
        print "There is something wrong with your request."
        print "Sending the server another request..."
        return reservation_request(url, slot_ID)

#===============================================================================

# Function to request a reservation for the clinet.
def cancel_reservation(url, slot_ID):
    # Get current time as the request ID, as it's always unique.
    current_ID = int(getTime() * 100)

    # Dictionary to store data that will be convert to XML string
    xmlData = {"request_ID": current_ID, "username": username,
               "password": password, "slot_ID": slot_ID}

    xmlTemplate = """<cancel>
<request_id>&&request_ID&&</request_id>
<username>&&username&&</username>
<password>&&password&&</password>
<slot_id>&&slot_ID&&</slot_id>
</cancel>"""

    for key in xmlData:
        oldValue = "&&" + str(key) + "&&"
        xmlTemplate = xmlTemplate.replace(oldValue, str(xmlData[key]))

    # Sending request
    request_result = requests.put(url, data=xmlTemplate, headers=headers)
    time.sleep(wait_time)

    # Checking the result status.
    if request_result.status_code == 200:
        print "Cancellation request was sent sucessfully"
        print "Request ID:", current_ID
        print "Slot ID:", slot_ID

        replyURL = (request_result.content + "?username=" + username
                    + "&password=" + password)
        replyURL = re.sub('<[^>]*>', '', replyURL)

        print replyURL

        server_result = requests.get(replyURL)

        # Check user login details.
        if server_result.status_code == 401:
            print "Error: 401", server_responses[401]
            return

        if server_result.status_code != 200:
            print "There was a problem processing the request"
            print "Processing again..."

        # While loop to get the correct out come of a successful request.
        while server_result.status_code != 200:
            server_result = requests.get(replyURL)
            time.sleep(wait_time)

        server_result_simple = re.sub('<[^>]*>', '', server_result.text)

        # Print out the result of the request.
        result_code = re.findall("<code>(.*?)</code>", server_result.text)

        if int(result_code[0]) != 200:
            print "Error", result_code[0], ":", server_responses[int(result_code[0])]
            print "Cancellation request unsuccessful please try again."
        else:
            print "Cancellation request successful."
        print "========================================================"
    else:
        print "There is something wrong with your request."
        print "Sending the server another request..."

        # Call the function again.
        cancel_reservation(url, slot_ID)

#===============================================================================
# Function to request all reservation of the client.
def get_bookings(url):
    # Get current time as the request ID, as it's always unique.
    current_ID = int(getTime() * 100)

    # Dictionary to store data that will be convert to XML string
    xmlData = {"request_ID": current_ID, "username": username,
               "password": password}

    xmlTemplate = """<bookings>
<request_id>&&request_ID&&</request_id>
<username>&&username&&</username>
<password>&&password&&</password>
</bookings>"""

    for key in xmlData:
        oldValue = "&&" + str(key) + "&&"
        xmlTemplate = xmlTemplate.replace(oldValue, str(xmlData[key]))

    # Sending request
    request_result = requests.put(url, data=xmlTemplate, headers=headers)
    time.sleep(wait_time)

    # Checking the result status.
    if request_result.status_code == 200:
        print "Booking request was sent sucessfully"
        print "Request ID:", current_ID

        replyURL = (request_result.content + "?username=" + username
                    + "&password=" + password)
        replyURL = re.sub('<[^>]*>', '', replyURL)

        print replyURL

        server_result = requests.get(replyURL)

        # Check user login details.
        if server_result.status_code == 401:
            print "Error: 401", server_responses[401]
            return

        if server_result.status_code != 200:
            print "There were an error processing the request"
            print "Processing again..."

        # While loop to get the correct out come of a successful request.
        while server_result.status_code != 200:
            server_result = requests.get(replyURL)
            time.sleep(wait_time)

        server_result_simple = re.sub('<[^>]*>', '\n', server_result.text)

        # Print out the result of the request.
        result_code = re.findall("<code>(.*?)</code>", server_result.text)

        if int(result_code[0]) != 200:
            print "Error", result_code[0], ":", server_responses[int(result_code[0])]
            print "Booking request unsuccessful please try again."
        else:
            print "Booking request successful."
            result_code = re.findall("<slot_id>(.*?)</slot_id>", server_result.text)
            if len(result_code) == 0:
                print "Your are currently not using any slot"
            else:
                for slot_id in result_code:
                    print "You are currently using slot:", slot_id
        print "========================================================"
    else:
        print "There is something wrong with your request."
        print "Sending the server another request..."
        get_bookings(url)

# Function to list all the available slots.
def free_slots(url):
    # Get current time as the request ID, as it's always unique.
    current_ID = int(getTime() * 100)

    # Dictionary to store data that will be convert to XML string
    xmlData = {"request_ID": current_ID, "username": username,
               "password": password}

    xmlTemplate = """<availability>
<request_id>&&request_ID&&</request_id>
<username>&&username&&</username>
<password>&&password&&</password>
</availability>"""

    for key in xmlData:
        oldValue = "&&" + str(key) + "&&"
        xmlTemplate = xmlTemplate.replace(oldValue, str(xmlData[key]))

    # Sending request
    request_result = requests.put(url, data=xmlTemplate, headers=headers)
    time.sleep(wait_time)

    # Checking the result status.
    if request_result.status_code == 200:
        print "Availability request was sent sucessfully"
        print "Request ID:", current_ID

        replyURL = (request_result.content + "?username=" + username
                    + "&password=" + password)
        replyURL = re.sub('<[^>]*>', '', replyURL)

        print replyURL

        server_result = requests.get(replyURL)

        # Check user login details.
        if server_result.status_code == 401:
            print "Error: 401", server_responses[401]
            return

        if server_result.status_code != 200:
            print "There were an error processing the request"
            print "Processing again..."

        # While loop to get the correct out come of a successful request.
        while server_result.status_code != 200:
            server_result = requests.get(replyURL)
            time.sleep(wait_time)

        server_result_simple = re.sub('<[^>]*>', '\n', server_result.text)

        # Print out the result of the request."You are currently using slot:", slot_id
        result_code = re.findall("<code>(.*?)</code>", server_result.text)

        if int(result_code[0]) != 200:
            print "Error", result_code[0], ":", server_responses[int(result_code[0])]
            print "Availability request unsuccessful please try again."
        else:
            print "Availability request successful."
            result_code = re.findall("<slot_id>(.*?)</slot_id>", server_result.text)
            return result_code
        print "========================================================"
    else:
        print "There is something wrong with your request."
        print "Sending the server another request..."
        free_slots(url)

# Function to return the array of available slot.
def get_slots():
    # List only slots that are available for both server.
    band_free_slot = free_slots(url_band)
    hotel_free_slot = free_slots(url_hotel)
    result = []

    for slot_band in band_free_slot:
        for slot_hotel in hotel_free_slot:
            if (slot_hotel == slot_band):
                result.append(slot_band)
            elif (int(slot_hotel) > int(slot_band)):
                break
    return result


# Function to start the client.
def start_client():
    print "=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+"
    print "                Welcome to the client!!!"
    print "=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+"

    # Get username and password
    print "Input your username and password below"
    #global username
    #username = raw_input("Username: ")
    #global password
    #password = raw_pass()

    while True:
        print "=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+"
        print "             Please chose an option below"
        print "=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+"
        print "0 - To exit the program."
        print "1 - To reserve a new slot."
        print "2 - To cancel a reservation slot."
        print "3 - To view all of your reserved slot."
        print "4 - To view all of the free slot available."

        choice = raw_input("Choice: ")
        if choice == '0':
            print "Thank you for using the client."
            print "Program exiting ..."
            print "********************************************************"
            exit()
        elif choice == '1':
            slot_list = get_slots()
            if slot_list is None:
                print "There are currently no available slots."
                print "Please try again."
            else
                slot_ID = str(slot_list[0])

                # Send request to band server.
                stage_one = reservation_request(url_band, slot_ID)
                time.sleep(wait_time)

                stage_two = 7

                if stage_one == -1:
                    print "Failed reservation request please try again."
                elif stage_one == 0:
                    stage_two = reservation_request(url_hotel, slot_ID)

                if stage_two == -1:
                    cancel_reservation(url_band, slot_ID)
                elif stage_two == 0 and stage_one == 0:
                    print "Reservation request completed!"



        elif choice == '2':
            # Cancel both slot reservation at the same time.
            print "Please input the slot ID you would like to cancel: "
            slot_ID = raw_input()

            cancel_reservation(url_band, slot_ID)
            cancel_reservation(url_hotel, slot_ID)
        elif choice == '3':
            get_bookings(url_band)
            get_bookings(url_hotel)
        elif choice == '4':
            # List only slots that are available for both server.
            band_free_slot = free_slots(url_band)
            hotel_free_slot = free_slots(url_hotel)

            for slot_band in band_free_slot:
                for slot_hotel in hotel_free_slot:
                    if (slot_hotel == slot_band):
                        print "Slots Available:", slot_band
                    elif (int(slot_hotel) > int(slot_band)):
                        break
        else:
            print "Invalid input please try again."
        print "********************************************************"
