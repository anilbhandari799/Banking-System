#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<map>
using namespace std;

#define MIN_BALANCE 500
class InsufficientFunds
{
	
};

class Account
{
	private:
		long account_number;
		string first_name;
		string last_name;
		string address;
		string email_id;
		float balance;
		static long next_account_number;
	public:
	    Account()
	    {
	    	
		}
		Account(string fname,string lname,string add,string email,float balance);
		long getAccount_no()
		{
			return account_number;
		}
		string getFirstname()
		{
			return first_name;
		}
		string getLastname()
		{
			return last_name;
		}
		string getAddress()
		{
			return address;
		}
		string getEmail()
		{
			return email_id;
		}
		float getBalance()
		{
			return balance;
		}
		void deposit(float amount);
		void withdraw(float amount);
		static void setLastAccountno(long account_number);
		static long getLastAccountno();
		friend ofstream & operator<<(ofstream &ofs,Account &acc);
		friend ifstream & operator>>(ifstream &ifs,Account &acc);
		friend ostream & operator<<(ostream &os,Account &acc);			
};
long Account::next_account_number=0;

class Bank
{
	private:
		map<long,Account>accounts;
	public:
	    Bank();
		Account OpenAccount(string fname,string lname,string add,string email,float bal);
		Account BalanceEnquiry(long account_number);
		Account deposit(long account_number,float amount);
		Account withdraw(long account_number,float amount);
		void CloseAccount(long account_number);
		void showAllaccounts();
		~Bank();			
};

Account::Account(string fname,string lname,string add,string email,float balance)
{
	next_account_number++;
	account_number=next_account_number;
	first_name=fname;
	last_name=lname;
	address=add;
	email_id=email;
	this->balance=balance;
}
void Account::deposit(float amount)
{
	balance+=amount;
}
void Account::withdraw(float amount)
{
	if(balance-amount<MIN_BALANCE)
	   throw InsufficientFunds();
	  balance-=amount; 
}

void Account::setLastAccountno(long account_number)
{
	next_account_number=account_number;
}

long Account::getLastAccountno()
{
	return next_account_number;
}

ofstream & operator<<(ofstream &ofs,Account &acc)
{
	ofs<<acc.account_number<<endl;
	ofs<<acc.first_name<<endl;
	ofs<<acc.last_name<<endl;
	ofs<<acc.address<<endl;
	ofs<<acc.email_id;
	ofs<<acc.balance<<endl;
	return ofs;
}


ifstream & operator>>(ifstream &ifs,Account &acc)
{
	ifs>>acc.account_number;
	ifs>>acc.first_name;
	ifs>>acc.last_name;
	ifs>>acc.address;
	ifs>>acc.email_id;
	ifs>>acc.balance;
	return ifs;
}


ostream & operator<<(ostream &os,Account &acc)
{
	os<<"First Name: "<<acc.getFirstname()<<endl;
    os<<"Last Name: "<<acc.getLastname()<<endl;
	os<<"Address: "<<acc.getAddress()<<endl;
	os<<"Email id: "<<acc.getEmail()<<endl;
	os<<"Account Number: "<<acc.getAccount_no()<<endl;
	os<<"Balance: "<<acc.getBalance()<<endl;
	return os;
}

Bank::Bank()
{
	Account account;
	ifstream infile;
	infile.open("Bank.data");
	if(!infile)
	{
		//cout<<"File is not opening";
		return ;
	}
	while(!infile.eof())
	{
		infile>>account;
		accounts.insert(pair<long,Account>(account.getAccount_no(),account));
	}
	Account::setLastAccountno(account.getAccount_no());
	infile.close();
}

Account Bank::OpenAccount(string fname,string lname,string add,string email,float balance)
{
	ofstream outfile;
	Account account(fname,lname,add,email,balance);
	accounts.insert(pair<long,Account>(account.getAccount_no(),account));
	
	outfile.open("Bank.data",ios::trunc);
	
	map<long,Account>::iterator itr;
	for(itr=accounts.begin();itr!=accounts.end();itr++)
	{
		outfile<<itr->second;
	}
	outfile.close();
	return account;	
}

Account Bank::BalanceEnquiry(long account_number)
{
	map<long,Account>::iterator itr=accounts.find(account_number);
	return itr->second;
}

Account Bank::deposit(long account_number,float amount)
{
	map<long,Account>::iterator itr=accounts.find(account_number);
	itr->second.deposit(amount);
	return itr->second;
}

Account Bank::withdraw(long account_number,float amount)
{
	map<long,Account>::iterator itr=accounts.find(account_number);
	itr->second.withdraw(amount);
	return itr->second;
}

void Bank::CloseAccount(long account_number)
{
	map<long,Account>::iterator itr=accounts.find(account_number);
	cout<<"Account Deleted\n"<<itr->second;
	accounts.erase(account_number);	
}

void Bank::showAllaccounts()
{
	map<long,Account>::iterator itr;
	for(itr=accounts.begin();itr!=accounts.end();itr++)
	{
		cout<<"Account "<<itr->first<<endl<<itr->second<<endl;
	}
}

Bank::~Bank()
{
	ofstream outfile;
	outfile.open("Bank.data",ios::trunc);
	
	map<long,Account>::iterator itr;
	for(itr=accounts.begin();itr!=accounts.end();itr++)
	{
		outfile<<itr->second;
	}
	outfile.close();
}

int main()
{
	Bank b;
	Account acc;
	
	int choice;
	string fname,lname,address,email;
	long account_number;
	float balance;
	float amount;
	cout<<"\n\n  *** B A N K I N G   S Y S T E M ***"<<endl;
	do
	{
		cout<<"\n\tSelect onr option below ";
		cout<<"\n\t1 Open an Account";
		cout<<"\n\t2 Balance Enquiry";
		cout<<"\n\t3 Deposit";
		cout<<"\n\t4 Withdrawal";
		cout<<"\n\t5 Close an Account";
		cout<<"\n\t6 Show All Account";
		cout<<"\n\t7 Quit\n";
		cout<<"\n Enter your choice: ";
		cin>>choice;
		switch(choice)
		{
			case 1:
				cout<<"Enter the first Name: ";
				cin>>fname;
				cout<<"Enter the Last Name: ";
				cin>>lname;
				cout<<"Enter the Address: ";
				cin>>address;
				cout<<"Enter the Email id: ";
				cin>>email;
				cout<<"Enter the initial Balance to deposit:\n ";
				cin>>balance;
				acc=b.OpenAccount(fname,lname,address,email,balance);
				cout<<endl<<"Congratulation Account is created"<<endl;
				cout<<acc;
				break;
				
			case 2:
			     cout<<"Enter the Account Number: ";
				 cin>>account_number;
				 acc=b.BalanceEnquiry(account_number);
				 cout<<endl<<"Your Account Details"<<endl;
				 cout<<acc;
				 break;
				 
			case 3:
			     cout<<"Enter Account Number: ";
				 cin>>account_number;
				 cout<<"Enter the balance to deposit: ";
				 cin>>amount;
				 acc=b.deposit(account_number,amount);
				 cout<<endl<<"Amount is Deposited"<<endl;
				 cout<<acc;
				 break;
				 
			case 4:
			     cout<<"Enter Account Number: ";
				 cin>>account_number;
				 cout<<"Enter the amount to withdraw: ";
				 cin>>amount;
				 acc=b.withdraw(account_number,amount);
				 cout<<endl<<"Amount withdrawn"<<endl;
				 cout<<acc;
				 break;
				 
			case 5:
			     cout<<"Enter Account Number:";
				 cin>>account_number;
				 b.CloseAccount(account_number);
				 cout<<endl<<"Account is closed"<<endl;
				 cout<<acc;
				 
			case 6:
			     b.showAllaccounts();
				 break;
				 
			case 7:
			      break;
			default:
			      cout<<"\nEnter Correct choice";
				  exit(0);	  	 	 	 	 	 	
		}
	}while(choice!=7);
	return 0;
}


