#include <kipr/botball.h>
#define COLOUR_ZONE_CONFIG "colour_zones.config"
#define POM_CONFIG "rgby.config"
#define RED_CHANNEL 0
#define BLUE_CHANNEL 1
#define YELLOW_CHANNEL 2
#define EMPTY -1
//Author: Luca Muscat
//Description: This will help the robot detect colour zones and where they are
//Wallaby Name:1978-Wallaby
//--------------------------
//The detected colour channels will be placed over here 
//-1 is going to be a null value
//Since there is no such thing as exception handling in c. The only way to detect if an element in array is populated by checking if its a null value
int colour_zone[3]={EMPTY,EMPTY,EMPTY};
int p_colour_zone[3]={EMPTY,EMPTY,EMPTY};
int tape_flag=0;
int populated=0;
int linear_search(int target[], int search_number){
	int i = 0;
    // "target[i]!=-1" was used to check if the element isnt null so that no errors would occur.
    for(i = 0;i<(int)sizeof(target)-1;i++){
    	if(search_number==target[i]){
        	//Pass
            return 1;
        }
    }
    //Fail
    return 0;
}
void drive_straight(){
    if(tape_flag<4){
	mav(0,700);
    mav(1,700);
    msleep(200);
    printf("drive straight");
    }else{
    	ao();
    }
}
int method_of_elimination(int target[]){
	int i =0;
    for(i=0;i<3;i++){
    	int j=0;
        for(j=0;j<2;j++){
        	if(i==target[j]){
            	break;
                continue;
            }else{
            	if(j==1){
                	return i;
                }
            }
        }
    }
}
int linear_index_check(int target[]){
	int c =0;
    for(c=0;c<3;c++){
    	if(target[c]==EMPTY){
            if(c==2){
				target[2]=method_of_elimination(colour_zone);
                return c;
            }
        	return c;
        }
    }
    return -1;
}
void print_array(int target[]){
	int i;
    for(i=0;i<(int)sizeof(target)-1;i++){
		printf("%d\n",target[i]);
    }
}

int main()
{
    thread tid;
    tid=thread_create(drive_straight);
    thread_start(tid);
	//printf("%d",linear_search(colour_zone,3));
    //Open the camera
    camera_open();
    //Load the appropriate colour zone
    camera_load_config(COLOUR_ZONE_CONFIG);
	//Update what the camera is seeing
    camera_update();
    int i = 0;
    int j=0;
        while(digital(8)==0 && analog(0)<2000){
            //This loop was used to iterate over the colour zones to see which one is detected
            int j=0;
            camera_update();
            for(i=0;i<3;i++){
                if(get_object_count(i)>0 && get_object_area(i,0)>5000){
                	camera_update();
                    int j=0;
                    //Checks for other colours
                    for(j=0;j<3;j++){
                    	if(j!=i){
                        	if(get_object_count(j)>0){
                            	break;
                            }else{
                            	continue;	
                            }
                        }else{
                        	continue;
                        }
                    }
                    unsigned int index = linear_index_check(colour_zone);
                    while(analog(0)<2000){
                    
                    }
                    if(colour_zone[1]!=EMPTY){
                    	thread_destroy(tid);
                        print_array(colour_zone);
                    }
                    
                }




            	msleep(500); 
                
        }
    }
    thread_destroy(tid);
    return 0;
}
