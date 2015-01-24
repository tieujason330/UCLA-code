//Return true if any of the array elements is negative, false
// otherwise.
//takes in an array of doubles and the size of the array
//uses recursion and return true if any of the array 
//elements is negative, false otherwise.
bool anyNegative(const double a[], int n)
{
	//if first element pointed at is less than zero return true
	if (a[0] < 0)
		return true;
	//if at last element and still greater than zero return false
	if (n == 1 && a[0] > 0)
		return false;
	//argument: pointing to next element after current one, size -1
	return anyNegative(a + 1, n - 1);
}

// Return the number of negative elements in the array using recursion
//takes in an array of doubles and size of the array
int countNegatives(const double a[], int n)
{
	//set cnt to zero
	int cnt = 0;
	//if gone through entire array, return 0
	if (n == 0)
		return 0;
	//if current first element is neg, add 1 to cnt
	if (a[0] < 0)
		cnt++;
	//go to next element
	return cnt + countNegatives(a + 1, n - 1);
}

// Return the subscript of the first negative element in the array.
// If no element is negative, return -1.
//takes in arguments array of doubles and size of array
int firstNegative(const double a[], int n)
{
	//if no negatives, then return -1
	if (n == 1 && a[0] > 0)
		return -1;
	//if first element is negative, then return 0
	if (a[0] < 0)
		return 0;
	//continues to the next element of the array
	return 1 + firstNegative(a + 1, n - 1);
}


// Return the subscript of the smallest element in the array.  If
// more than one element has the same smallest value, return the
// smallest subscript of such an element.  If the array has no
// elements to examine, return -1.
int indexOfMin(const double a[], int n)
{
	if (n == 0) return -1; //if the array has no elements to examine
	if (n < 2) //if at last element then it is the smallest
		return 0;
	if (a[0] < a[1 + indexOfMin(a + 1, n - 1)])//checks to see if current element is smallest
		return 0;
	else 
		return 1 + indexOfMin(a + 1, n - 1); //continues onto next element
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20

//takes in two arrays of doubles and two ints representing their respective sizes
bool includes(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 <= 0) //checks to see if is at the end of the searching array
		return true;
	if (n1 <= 0)//checks to see if at end of the array being searched
		return false;

	if (a1[0] == a2[0])
		return (includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1));//keeps searching using next elements
	else
		return (includes(a1 + 1, n1 - 1, a2, n2));//goes to next for a2, stays at same for a1
}