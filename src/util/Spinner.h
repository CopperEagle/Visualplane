#ifndef SPINNER_H
#define SPINNER_H

/*!
 * @brief Progress bar element. Use it to report progress to the user.
 * It is pretty basic at this point.
 */
class Spinner
{
public:
	Spinner(unsigned n);
	~Spinner(){};

	void start();
	void next();
private:
	const unsigned maximum;
	unsigned curr = 0;
	
};

#endif 