#ifndef FRAMERATEENFORCER_H
#define FRAMERATEENFORCER_H

/*!
 * @brief Enforces that a particular point of the code can only be revisited
 * a given maximum number of times per second. Usually used to enforce a maximum
 * iterations for a (main) loop.
 */
class FrameRateEnforcer
{
public:
	/*!
	* @brief Constructor of the FrameRateEnforcer.
	* 
	* @param[in] fps The targeted (maximum) frames per second
	*/
	FrameRateEnforcer(double fps);
	~FrameRateEnforcer(){};

	/*!
	* @brief Notify that the loop, which requires the fps-limit.
	*/
	void startloop();
	/*!
	* @brief Make sure that 1.0s/fps seconds have passed (or more) since the last
	* call to either startloop or wait on this enforcer-object.
	* 
	* If you call wait() once per loop, this will ensure that your declared fps will not 
	* be exceeded.
	*/
	void wait();
private:
	const double wait_period;
	double timestamp;
};



#endif // FRAMERATEENFORCER_H