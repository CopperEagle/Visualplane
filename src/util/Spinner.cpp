#include <iostream>
#include "Spinner.h"

/*!
 *	@brief Constructor of the Spinner class.
 * 	@param[in] n Which is the number of substeps that need to be taken to complete the process.
 *  
 */
Spinner::Spinner(unsigned n) : maximum(n){}

/*!
 * @brief Start the processing. This will show a progress bar at 0%.
 */
void Spinner::start()
{
	std::cout << "---------- 0% ...starting" << std::flush;
}

/*!
 * @brief Notify the Spinner object that another substep has been *completed*.
 */
void Spinner::next()
{
	curr++;
	int contrib = int(10*float(curr)/maximum);
	std::cout << '\r' << std::string(contrib, '#') << std::string(10 - contrib, '-') << ' '
				<< 100*float(curr)/maximum << '%' << std::flush;
}