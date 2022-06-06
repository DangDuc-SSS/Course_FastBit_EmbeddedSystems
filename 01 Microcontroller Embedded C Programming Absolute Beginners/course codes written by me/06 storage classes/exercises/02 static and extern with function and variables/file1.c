extern int mainPrivateData;
extern void change_system_clock(int system_clock);
void file1_myFun1(void)
{
	mainPrivateData = 9000;

	// commenting this line will throw error because this function is private
	//change_system_clock(0); 
}