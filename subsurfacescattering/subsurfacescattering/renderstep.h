#ifndef RENDERSTEP_H
#define RENDERSTEP_H

class RenderStep
{
public:

	~RenderStep();
private:
	RenderStep();
	RenderStep(RenderStep const&){};
	RenderStep& operator=(RenderStep const&){};

	virtual void initialize() = 0;

private:
	
};

#endif // RENDERSTEP_H
