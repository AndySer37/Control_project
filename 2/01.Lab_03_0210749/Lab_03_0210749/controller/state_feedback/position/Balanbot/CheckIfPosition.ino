bool CheckIfPosition()
{
	if(str_receive[0] == '[' && str_receive[1] == 'p' && str_receive[2] == 'o' && str_receive[3] == 's' && str_receive[4] == ':')
    {
    	char* tempPos = (char*)malloc(len_receive-4);
    	for (int i = 5;i < len_receive;i++)
    	{
    		tempPos[i - 5] = str_receive[i];
    	}

    	target_car_position = atof(tempPos) + SHIFT;

    	free(tempPos);

    	BTSerial2.println(target_car_position);

    	return true;
    }
    return false;
	
}