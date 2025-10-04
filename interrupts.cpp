/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include<interrupts.hpp>

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    int time_count = 0;


    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU")
        {
            execution += std:: to_string(time_count) + ", " + std:: to_string(duration_intr) + ", CPU Burst\n"; 
            time_count += duration_intr;

        }

        else if ((activity == "SYSCALL") && duration_intr > 19)
        {
            execution += "Device number does not exist\n";
        }

        else if (activity == "SYSCALL")
        {

            auto [ISR_output, ISR_duration] = intr_boilerplate(time_count, duration_intr, 1, vectors);
            execution += ISR_output;
            time_count = ISR_duration;

            execution+= std:: to_string(time_count) + ", " + std:: to_string(40) + ", SYSCALL: run the ISR (device driver)\n";
            time_count += 40;

            execution+= std:: to_string(time_count) + ", " + std:: to_string(40) + ", transfer data from device to memory\n";
            time_count += 40;
                                    
            execution += std:: to_string(time_count) + ", " + std:: to_string(delays.at(duration_intr)) + " check for errors" + "\n";
            time_count += delays.at(duration_intr);

            execution+= std:: to_string(time_count) + ", " + std:: to_string(1) + ", IRET\n";
            time_count ++;
            
        }

        else if ((activity == "END_IO") && duration_intr > 19)
        {
            execution += "Device number does not exist\n";
        }

        else if (activity == "END_IO")
        {
           
            auto [ENDIO_output, ENDIO_duration] = intr_boilerplate(time_count, duration_intr, 1, vectors);
            execution += ENDIO_output;
            time_count = ENDIO_duration;

            execution+= std:: to_string(time_count) + ", " + std:: to_string(40) + ", ENDIO: run the ISR (device driver)\n";
            time_count += 40;

            execution += std:: to_string(time_count) + ", " + std:: to_string(delays.at(duration_intr)) + " check device status" + "\n";
            time_count += delays.at(duration_intr);

            execution+= std:: to_string(time_count) + ", " + std:: to_string(1) + ", IRET\n";
            time_count ++;

        }

        
        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
