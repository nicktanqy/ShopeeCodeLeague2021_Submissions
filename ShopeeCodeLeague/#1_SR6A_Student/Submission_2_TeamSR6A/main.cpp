#include "RapidJSON/rapidjson.h"
#include "RapidJSON/document.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>

struct Ticket
{
	int ID;
	std::string email;
	std::string phone;
	int contacts;
	std::string orderID;
	std::set<int> combinedID;
};

int main() {

	//Start Read in Input
	std::string fileName("contacts.json");
	std::vector<Ticket> allTickets;

	std::ifstream inFile;
	std::stringstream allDataStream;

	inFile.open(fileName, std::ios::binary);
	allDataStream << inFile.rdbuf() << std::endl;

	inFile.close();

	rapidjson::Document d;
	d.Parse(allDataStream.str().c_str());
	if (!d.IsArray())
	{
		return -1;
	}

	for (rapidjson::SizeType i = 0; i < d.Size(); i++)
	{
		Ticket newTicket;
		if (d[i].HasMember("Id")) {
			rapidjson::Value& val = d[i]["Id"];
			newTicket.ID = val.GetInt();
		}
		if (d[i].HasMember("Email")) {
			rapidjson::Value& val = d[i]["Email"];
			newTicket.email = val.GetString();
		}
		if (d[i].HasMember("Phone")) {
			rapidjson::Value& val = d[i]["Phone"];
			newTicket.phone = val.GetString();
		}
		if (d[i].HasMember("Contacts")) {
			rapidjson::Value& val = d[i]["Contacts"];
			newTicket.contacts = val.GetInt();
		}
		if (d[i].HasMember("OrderId")) {
			rapidjson::Value& val = d[i]["OrderId"];
			newTicket.orderID = val.GetString();
		}
		newTicket.combinedID.insert(newTicket.ID);
		allTickets.push_back(newTicket);
	}
	//End Read in Input


	//Start Processing Data
	for (size_t i = 0; i < allTickets.size(); ++i) {
		//Processing Order ID
		if (allTickets[i].orderID != "") {
			for (size_t j = i + 1; j < allTickets.size(); ++j) {
				if (allTickets[i].orderID == allTickets[j].orderID) {
					for (auto& tempID : allTickets[i].combinedID) {
						allTickets[j].combinedID.insert(tempID);
						allTickets[tempID].combinedID.insert(j);
					}
					for (auto& tempID : allTickets[j].combinedID) {
						allTickets[i].combinedID.insert(tempID);
						allTickets[tempID].combinedID.insert(i);
					}
				}
			}
		}
		//Processing Email
		if (allTickets[i].email != "") {
			for (size_t j = i + 1; j < allTickets.size(); ++j) {
				if (allTickets[i].email == allTickets[j].email) {
					for (auto& tempID : allTickets[i].combinedID) {
						allTickets[j].combinedID.insert(tempID);
						allTickets[tempID].combinedID.insert(j);
					}
					for (auto& tempID : allTickets[j].combinedID) {
						allTickets[i].combinedID.insert(tempID);
						allTickets[tempID].combinedID.insert(i);
					}
				}
			}
		}
		//Processing Phone
		if (allTickets[i].phone != "") {
			for (size_t j = i + 1; j < allTickets.size(); ++j) {
				if (allTickets[i].phone == allTickets[j].phone) {
					for (auto& tempID : allTickets[i].combinedID) {
						allTickets[j].combinedID.insert(tempID);
						allTickets[tempID].combinedID.insert(j);
					}
					for (auto& tempID : allTickets[j].combinedID) {
						allTickets[i].combinedID.insert(tempID);
						allTickets[tempID].combinedID.insert(i);
					}
				}
			}
		}
	}
	//End Processing Data

	//Start Output
	std::stringstream outDataStream;
	//Title
	outDataStream << "ticket_id,ticket_trace/contact" << std::endl;
	//Loop
	for (size_t i = 0; i < allTickets.size(); ++i) {
		//Counting contacts and formatting the output
		std::string combinedIDFormat;
		int totalContacts = 0;
		for (auto& tempID : allTickets[i].combinedID) {
			combinedIDFormat += std::to_string(tempID) + "-";
			totalContacts += allTickets[tempID].contacts;
		}
		if (combinedIDFormat.size() > 1)
			combinedIDFormat = combinedIDFormat.substr(0, combinedIDFormat.size() - 1);
		outDataStream << allTickets[i].ID << ",\"" << combinedIDFormat << "," << totalContacts << "\"" << std::endl;
	}
	fileName = "output.csv";
	std::ofstream outFile;
	outFile.open(fileName, std::ios::binary);
	outFile << outDataStream.str();
	outFile.close();

	//End Output
	return 0;
}