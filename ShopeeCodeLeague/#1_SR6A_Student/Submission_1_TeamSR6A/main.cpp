#include "RapidJSON/rapidjson.h"
#include "RapidJSON/document.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <set>
#include <memory>
/*
[{"Id": 0, "Email" : "gkzAbIy@qq.com", "Phone" : "", "Contacts" : 1, "OrderId" : ""},
{ "Id": 1, "Email" : "", "Phone" : "329442681752", "Contacts" : 4, "OrderId" : "vDDJJcxfLtSfkooPhbYnJdxov" },
{ "Id": 2, "Email" : "", "Phone" : "9125983679", "Contacts" : 0, "OrderId" : "" },
{ "Id": 3, "Email" : "mdllpYmE@gmail.com", "Phone" : "", "Contacts" : 0, "OrderId" : "bHquEnCbbsGLqllwryxPsNOxa" }]
*/
using namespace std;

struct Ticket
{
	int ID;
	std::string email;
	std::string phone;
	int contacts;
	std::string orderID;
	set<int> combinedID;
};

std::unordered_map<string, pair<set<int>, int>> emails;
std::unordered_map<string, pair<set<int>, int>> phones;
std::unordered_map<string, pair<set<int>, int>> orderIDs;


int main() {

	//Read in Input
	std::string fileName("contacts.json");
	std::vector<std::unique_ptr<Ticket>> allTickets;

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
		auto newTicket = std::make_unique<Ticket>();
		if (d[i].HasMember("Id")) {
			rapidjson::Value& val = d[i]["Id"];
			newTicket->ID = val.GetInt();
		}
		if (d[i].HasMember("Email")) {
			rapidjson::Value& val = d[i]["Email"];
			newTicket->email = val.GetString();
			if (!newTicket->email.empty())
			{
				emails[newTicket->email].first.insert(newTicket->ID);
				emails[newTicket->email].second += newTicket->contacts;
			}
		}
		if (d[i].HasMember("Phone")) {
			rapidjson::Value& val = d[i]["Phone"];
			newTicket->phone = val.GetString();
			if (!newTicket->phone.empty())
			{
				phones[newTicket->phone].first.insert(newTicket->ID);
				phones[newTicket->phone].second += newTicket->contacts;
			}
		}
		if (d[i].HasMember("Contacts")) {
			rapidjson::Value& val = d[i]["Contacts"];
			newTicket->contacts = val.GetInt();
		}
		if (d[i].HasMember("OrderId")) {
			rapidjson::Value& val = d[i]["OrderId"];
			newTicket->orderID = val.GetString();

			if (!newTicket->orderID.empty())
			{
				orderIDs[newTicket->orderID].first.insert(newTicket->ID);
				orderIDs[newTicket->orderID].second += newTicket->contacts;
			}
		}
		allTickets.push_back(std::move(newTicket));
	}
	//End Read in Input

	// For each ticket
	for (auto& ticket : allTickets)
	{
		// If email exists already
		auto email = emails.find(ticket->email);
		if (email != emails.end())
		{
			auto& ids = email->second.first;
			ticket->combinedID.insert(ids.begin(), ids.end());
			for (auto& id : ids) {
				allTickets[id]->combinedID.insert(ticket->ID);
				for (auto& id2 : allTickets[id].get()->combinedID)
				{
					allTickets[id2]->combinedID.insert(ticket->ID);
					ticket->combinedID.insert(allTickets[id2]->ID);
					for (auto& id3 : allTickets[id2].get()->combinedID)
					{
						allTickets[id3]->combinedID.insert(ticket->ID);
					}
				}
			}
		}

		// If email exists already
		auto phone = phones.find(ticket->phone);
		if (phone != phones.end())
		{
			auto& ids = phone->second.first;
			ticket->combinedID.insert(ids.begin(), ids.end());
			for (auto& id : ids) {
				allTickets[id]->combinedID.insert(ticket->ID);
				for (auto& id2 : allTickets[id].get()->combinedID)
				{
					allTickets[id2]->combinedID.insert(ticket->ID);
					ticket->combinedID.insert(allTickets[id2]->ID);
					for (auto& id3 : allTickets[id2].get()->combinedID)
					{
						allTickets[id3]->combinedID.insert(ticket->ID);
					}
				}
			}
		}

		// If email exists already
		auto orderID = orderIDs.find(ticket->orderID);
		if (orderID != orderIDs.end())
		{
			auto& ids = orderID->second.first;
			ticket->combinedID.insert(ids.begin(), ids.end());
			for (auto& id : ids) {
				allTickets[id]->combinedID.insert(ticket->ID);
				for (auto& id2 : allTickets[id].get()->combinedID)
				{
					allTickets[id2]->combinedID.insert(ticket->ID);
					ticket->combinedID.insert(allTickets[id2]->ID);
					for (auto& id3 : allTickets[id2].get()->combinedID)
					{
						allTickets[id3]->combinedID.insert(ticket->ID);
					}
				}
			}
		}
	}

	//Start Output
	std::stringstream outDataStream;
	//Title
	outDataStream << "ticket_id,ticket_trace/contact" << std::endl;
	//Loop
	for (size_t i = 0; i < allTickets.size(); ++i) {
		string comb;
		int totalContacts = 0;
		for (auto j : allTickets[i]->combinedID)
		{
			comb += to_string(j);
			comb += "-";


			totalContacts += allTickets[j]->contacts;
		}
		comb = comb.substr(0, comb.size() - 1);
		outDataStream << allTickets[i]->ID << ",\"" << comb << "," << totalContacts << "\"" << std::endl;
	}
	fileName = "output.csv";
	std::ofstream outFile;
	outFile.open(fileName, std::ios::binary);
	outFile << outDataStream.str();
	outFile.close();

	return 0;
}
/*
[{"Id": 0, "Email" : "gkzAbIy@qq.com", "Phone" : "", "Contacts" : 1, "OrderId" : ""},
{ "Id": 1, "Email" : "", "Phone" : "329442681752", "Contacts" : 4, "OrderId" : "vDDJJcxfLtSfkooPhbYnJdxov" },
{ "Id": 2, "Email" : "", "Phone" : "9125983679", "Contacts" : 0, "OrderId" : "" },
{ "Id": 3, "Email" : "mdllpYmE@gmail.com", "Phone" : "", "Contacts" : 0, "OrderId" : "bHquEnCbbsGLqllwryxPsNOxa" }]
*/