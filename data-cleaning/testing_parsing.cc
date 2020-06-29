
#include <iostream>

int main(){
  std::ifstream myFile("cleaned_data_10EVENTS_5ACTIONS/test_data.csv");
  if(!myFile.is_open()) throw std::runtime_error("Could not open file");

  int num_events = 10;
  std::vector<uint64_t> pc;
  std::vector<std::vector<uint8_t>> signatures;
  std::vector<int> action;
  std::string file_line,col_value;
  while(myFile.good()){
    getline(myFile,file_line);
    std::stringstream ss(file_line);

    if (!(std::getline(ss, col_value, ','))){ //checking if its an empty line
      continue;
    }
    action.push_back(std::stoi(col_value));

    std::getline(ss, col_value, ',');
    pc.push_back(std::stoi(col_value,0,16));

    std::vector<uint8_t> current_signature;
    int count = 0;
    while(std::getline(ss, col_value, ',')){
      int event_int = std::stoi(col_value);
      uint8_t event_uint8 = event_int;
      current_signature.push_back(event_uint8);
      //cout<<std::stoi(col_value)<<endl;
      count++;
      if(count>num_events){
        cout<<"Number of Events specified is less than number of events that exist"<<endl;
        return 1;
      }
    }
    if(count!=num_events){
        cout<<"Number of Events specified is less than the number that exist"<<endl;
        return 1;
    }
    signatures.push_back(current_signature);
  }
  for(int i=0;i<pc.size();i++){
    cout<<pc[i]<<" "<<action[i]<< " "<<endl;
    for(int j=0; j<num_events;j++){
      cout<<unsigned(signatures[i][j])<<endl;
    }
  }
  return 1;
}