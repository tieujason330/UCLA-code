//two-parameter overload of listAll that takes in a Class pointer and a string
//uses recursion to print out full path to all subclasses
void listAll(const Class* c, string path)  // two-parameter overload
{
	//first print out the class being passed though
	string mainClass = c->name();
	cout << path << mainClass << endl;

	//check for subclasses
	//for subClassPath append previous path with mainClass 
	//use recursion
	for (int i = 0; i < c->subclasses().size(); i++)
	{
		string subClassPath = path + mainClass + "=>";
		listAll(c->subclasses()[i], subClassPath);
	}
}
