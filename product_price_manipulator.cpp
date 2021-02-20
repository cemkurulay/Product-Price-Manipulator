#include <bits/stdc++.h>

using namespace std;

class product {       
  public:             
    string code;        
    int price;  
    int stock;
    vector<string> campaign_names;
};

class campaign {       
  public:             
    string name;        
    string product_code;
    int start_time;
    int duration;
    int price;
    int limit;
    int total_sales = 0;
    int turnover = 0;
    int target_sales_count;
};

string getTime(int time){
    if(time < 10){
        return "0" + to_string(time) + ":00";
    }else{
        return to_string(time) + ":00";
    }
}

string getCampaignStatus(int cur_time, int campaign_start, int campaign_duration){
    // if campaign is supposed to be active at the end of the duration time, the "<" sign below can be changed to "<=" 
    if(cur_time < campaign_start+campaign_duration){
        return "Active";
    }else{
        return "Ended";
    }
}

void campaignOrder(campaign active_campaign, product ordered_product, int quantity, unordered_map<string, campaign>& campaigns, unordered_map<string, product>& products){
    int calculated_price = active_campaign.price;
    int original_price = ordered_product.price;
    int limit = active_campaign.limit;
    active_campaign.total_sales += quantity;
    active_campaign.turnover += calculated_price * quantity;
    calculated_price += quantity*5;
    if(calculated_price > original_price+limit){
        calculated_price = original_price+limit;
    }
    active_campaign.price = calculated_price;
    campaigns[active_campaign.name] = active_campaign;
}

void updateCampaignPrice(campaign active_campaign, int time_increase, unordered_map<string, campaign>& campaigns, int original_price){
    int calculated_price = active_campaign.price;
    int limit = active_campaign.limit;
    calculated_price -= time_increase*5;
    if(calculated_price < original_price - limit){
        calculated_price = original_price - limit;
    }
    active_campaign.price = calculated_price;
    campaigns[active_campaign.name] = active_campaign;
}

int main(){
    string cmd;
    int time =0;
    unordered_map<string, product> products;
    unordered_map<string, campaign> campaigns;


    while(true){
        cin>>cmd;
        if(cmd == "create_product"){
            product new_product;
            cin>>new_product.code>>new_product.price>>new_product.stock;
            products[new_product.code]=new_product;
            string res = "Product created; code " + new_product.code + ", price " + to_string(new_product.price) + ", stock " + to_string(new_product.stock);
            cout<<res<<endl;
        }else if(cmd == "create_campaign"){
            campaign new_campaign;
            cin>>new_campaign.name>>new_campaign.product_code>>new_campaign.duration>>new_campaign.limit>>new_campaign.target_sales_count;
            new_campaign.start_time = time;   
            new_campaign.total_sales = 0;
            new_campaign.turnover = 0;
            new_campaign.price = products[new_campaign.product_code].price;
            campaigns[new_campaign.name]=new_campaign;
            products[new_campaign.product_code].campaign_names.push_back(new_campaign.name);
            string res = "Campaign created; name " + new_campaign.name + ", product " + new_campaign.product_code + ", duration " + to_string(new_campaign.duration) + ", limit " + to_string(new_campaign.limit) + ", target sales count " + to_string(new_campaign.target_sales_count) ;
            cout<<res<<endl;
        }else if(cmd == "get_product_info"){
            string product_code;
            cin>>product_code;
            product temp_product;
            temp_product = products[product_code];
            campaign temp_campaign;
            int current_price = temp_product.price;
            for(auto campaign_name : products[product_code].campaign_names){
                temp_campaign = campaigns[campaign_name];
                if(getCampaignStatus(time, temp_campaign.start_time, temp_campaign.duration) == "Active"){
                    current_price = temp_campaign.price;
                }
            }
            string res = "Product " + product_code + " info; price " + to_string(current_price) + ", stock " + to_string(temp_product.stock);
            cout<<res<<endl;
        }else if(cmd == "increase_time"){
            int time_increase;
            cin>>time_increase;
            time += time_increase;
            for(auto campaign : campaigns){
                if(getCampaignStatus(time, campaign.second.start_time, campaign.second.duration) == "Active"){
                    int product_price = products[campaign.second.product_code].price;
                    updateCampaignPrice(campaign.second, time_increase, campaigns, product_price);
                }
            }
            cout<<"Time is "<<getTime(time)<<endl;
        }else if(cmd == "get_campaign_info"){
            string campaign_code;
            cin>>campaign_code;
            campaign temp_campaign;
            temp_campaign = campaigns[campaign_code];
            string res = "Campaign " + campaign_code + " info; Status " + getCampaignStatus(time, temp_campaign.start_time, temp_campaign.duration) + ", Target Sales " + to_string(temp_campaign.target_sales_count) + ", Total Sales " + to_string(temp_campaign.total_sales) + ", Turnover " + to_string(temp_campaign.turnover) + ", Average Item Price " + to_string((temp_campaign.turnover*1.0)/temp_campaign.total_sales);
            cout<<res<<endl;
        }else if(cmd == "create_order"){
            string product_code;
            int quantity;
            campaign temp_campaign;
            cin>>product_code>>quantity;
            products[product_code].stock -= quantity;
            for(auto campaign_name : products[product_code].campaign_names){
                temp_campaign = campaigns[campaign_name];
                if(getCampaignStatus(time, temp_campaign.start_time, temp_campaign.duration) == "Active"){
                    campaignOrder(temp_campaign, products[product_code], quantity, campaigns, products);
                }
            }
            cout<<"Order created; product "<<product_code<<", quantity "<<quantity<<endl;
        }else{
            cout<<"Wrong command, please try again:"<<endl;
        }
    }
    return 0;
}