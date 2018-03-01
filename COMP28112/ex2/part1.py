from time import time as getTime
import time
import requests
import re

# The hotel's server url.
url = "http://jewel.cs.man.ac.uk:3010/queue/enqueue"
url2 = "http://jewel.cs.man.ac.uk:3020/queue/enqueue"

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
username = "mn2114"
password = "bMr879"

# Function to request a reservation for the client.
def reservation_request():
    # Get current time as the request ID, as it's always unique.
    current_ID = int(getTime() * 100)

    print "Please input the slot ID you would like to reserve: "
    slot_ID = raw_input()

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
            print server_result_simple[0], server_result_simple[1]
            print "Error", result_code[0], ":", server_responses[int(result_code[0])]
            print "Reservation request unsuccessful please try again."
        else:
            print "Reservation request successful."
        print "============================================================"
    else:
        print "There is something wrong with your request."
        print "Sending the server another request..."

        while request_result.status_code != 200:
            request_result = requests.put(url, data=xmlTemplate, headers=headers)
        reservation_request()

#===============================================================================

# Function to request a reservation for the clinet.
def cancel_reservation():
    # Get current time as the request ID, as it's always unique.
    current_ID = int(getTime() * 100)

    print "Please input the slot ID you would like to cancel: "
    slot_ID = raw_input()

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
            print server_result_simple[0], server_result_simple[1]
            print "Error", result_code[0], ":", server_responses[int(result_code[0])]
            print "Cancellation request unsuccessful please try again."
        else:
            print "Cancellation request successful."
        print "============================================================"
    else:
        print "There is something wrong with your request."
        print "Sending the server another request..."

        while request_result.status_code != 200:
            request_result = requests.put(url, data=xmlTemplate, headers=headers)
        cancel_reservation()

# Function to request a reservation for the clinet.
def get_bookings():
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

    # Checking the result status.
    if request_result.status_code == 200:
        print "Reservation request was sent sucessfully"
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

        if result_code[0] != 200:
            print server_result_simple
            print "Error", result_code[0], ":", server_responses[int(result_code[0])]
            print "Reservation request unsuccessful please try again."
        else:
            print "Reservation request successful."
        print "============================================================"
    else:
        print "There is something wrong with your request."
        print "Sending the server another request..."

        while request_result.status_code != 200:
            request_result = requests.put(url, data=xmlTemplate, headers=headers)
        get_bookings()

# Function to request a reservation for the clinet.
def free_slots():
    # Get current time as the request ID, as it's always unique.
    current_ID = int(getTime() * 100)

    print "Please input the slot ID you would like to reserve: "
    slot_ID = raw_input()

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
            print "There were an error processing the request"
            print "Processing again..."

        # While loop to get the correct out come of a successful request.
        while server_result.status_code != 200:
            server_result = requests.get(replyURL)
            time.sleep(wait_time)

        server_result_simple = re.sub('<[^>]*>', '\n', server_result.text)

        # Print out the result of the request.
        result_code = re.findall("<code>(.*?)</code>", server_result.text)

        if result_code[0] != 200:
            print server_result_simple
            print "Error", result_code[0], ":", server_responses[int(result_code[0])]
            print "Reservation request unsuccessful please try again."
        else:
            print "Reservation request successful."
        print "============================================================"
    else:
        print "There is something wrong with your request."
        print "Sending the server another request..."

        while request_result.status_code != 200:
            request_result = requests.put(url, data=xmlTemplate, headers=headers)
        reservation_request()

def start_server():
    return
