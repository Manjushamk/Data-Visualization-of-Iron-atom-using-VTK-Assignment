/*Author : Mani Manjusha Kottala
Subject: Data Visualization
Project 2: Interactive VTK program
*/

//header files for all the objects used in the program
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkStructuredPointsReader.h>
#include <vtkContourFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSliderWidget.h>
#include <vtkSliderWidget.h>
#include <vtkSliderRepresentation.h>
#include <vtkSliderRepresentation2D.h>
#include <vtkInteractorStyleUser.h>
#include <vtkCamera.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkImageData.h>
#include <vtkLookupTable.h>
#include <vtkMarchingCubes.h>
#include <vtkTextWidget.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>



/*--------------call back class for the iso-values Slider Widget--------------*/

class vtkSliderCallback : public vtkCommand
{
public:

	//slider call back constructer for iso-values slider
	static vtkSliderCallback *New()
	{
		return new vtkSliderCallback;
	}

	//function execution on iso-values slider movement
	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		//assigning the iso-values for iso-surface,cross-section and contour liness
		vtkSliderWidget *slider = reinterpret_cast<vtkSliderWidget*>(caller);
		int iso_val = static_cast<vtkSliderRepresentation *>(slider->GetRepresentation())->GetValue();
		this->sliderInteracter->SetValue(0, iso_val);
		this->slidercutter->SetValue(0,iso_val);
	}

	vtkSliderCallback() :sliderInteracter(NULL) {}
	vtkContourFilter* sliderInteracter;
	vtkCutter* slidercutter;
};


/*--------------call back class for cross-section slider Widget--------------*/
class vtkSliderCallback_cross : public vtkCommand
{
public:

	//slider call back constructer for cross-section slider
	static vtkSliderCallback_cross *New()
	{
		return new vtkSliderCallback_cross;
	}

	//function execution on cross-section slider movement
	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		//assigning the cross-section values for iso-surface,cross-section and contour liness
		vtkSliderWidget *slider_cross = reinterpret_cast<vtkSliderWidget*>(caller);
		int cross_val = static_cast<vtkSliderRepresentation *>(slider_cross->GetRepresentation())->GetValue();
		this->sliderInteracter_cross->SetOrigin(cross_val, 0, 0);
	}

	vtkSliderCallback_cross() :sliderInteracter_cross(NULL) {}
	vtkPlane* sliderInteracter_cross;
};


/*--------------start of main function--------------*/

int main(int argc, char* argv[])
{

	//input args
	if (argc != 2)
	{
		cout << "Failure";
		return EXIT_FAILURE;
	}

	std::string Filename = argv[1];
	//reading the ironProt.vtk file

	vtkSmartPointer<vtkStructuredPointsReader> file_reader = vtkSmartPointer<vtkStructuredPointsReader>::New();
	file_reader->SetFileName(Filename.c_str());

	//file_reader->SetFileName("C:/Users/Manju/Documents/Visual Studio 2013/Projects/iron_data/src/ironProt.vtk");
	file_reader->Update();

	//------------set up iso-surfaces-------------
	// filtering: using contourFilter for getting the iso-surface
	vtkSmartPointer <vtkContourFilter> isosurface_rep = vtkSmartPointer <vtkContourFilter>::New();
	isosurface_rep->SetInputConnection(file_reader->GetOutputPort());
	isosurface_rep->SetValue(0, 50);

	// cleaning up for removing the duplicates
	vtkSmartPointer<vtkCleanPolyData> clean_up = vtkSmartPointer<vtkCleanPolyData>::New();
	clean_up->SetInputConnection(isosurface_rep->GetOutputPort());


	//normals for polygonal mesh structure to create soft edges for the differangle of edges till feature angle
	vtkSmartPointer<vtkPolyDataNormals> poly_normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	poly_normals->SetInputConnection(clean_up->GetOutputPort());
	poly_normals->SetFeatureAngle(45);

	// map the polygonal data to the graphics
	vtkSmartPointer<vtkPolyDataMapper> dataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	dataMapper->SetInputConnection(clean_up->GetOutputPort());
	dataMapper->ScalarVisibilityOff();

	// creating vtk actor for the iso-surfaces object
	vtkSmartPointer<vtkActor> isoActor = vtkSmartPointer<vtkActor>::New();
	isoActor->SetMapper(dataMapper);
	isoActor->GetProperty()->SetColor(1.0, 0.0, 0.1);
	isoActor->GetProperty()->SetOpacity(0.9);

	

	// -------------Setup cross section----------------

	// Create plane by defining the origin and normals
	vtkSmartPointer<vtkPlane> crossPlane = vtkSmartPointer<vtkPlane>::New();
	crossPlane->SetOrigin(10, 0, 0);
	crossPlane->SetNormal(50, 0, 0);


	// Create cutter and filereader output is given as input to the cutter 
	vtkSmartPointer<vtkCutter> crossCutter = vtkSmartPointer<vtkCutter>::New();
	crossCutter->SetCutFunction(crossPlane);
	crossCutter->SetInputConnection(file_reader->GetOutputPort());
	crossCutter->Update();
	crossCutter->SetValue(0, 50);
	
	// Creating a lookup table.
	/*vtkSmartPointer<vtkLookupTable> lookup_col = vtkSmartPointer<vtkLookupTable>::New();
	lookup_col->SetNumberOfColors(16);
	lookup_col->SetHueRange(0.0, 0.667);
	lookup_col->SetSaturationRange(0.0, 0.7);
	lookup_col->SetRange(0, 128);
	lookup_col->Build();*/

	// map the polygonal data to the graphics
	vtkSmartPointer<vtkPolyDataMapper> crossPolyDataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	crossPolyDataMapper->SetInputConnection(crossCutter->GetOutputPort());
	//crossPolyDataMapper->SetLookupTable(lookup_col);
	//crossPolyDataMapper->SetScalarRange(0,128);

	// creating vtk actor for cross-section plane
	vtkSmartPointer<vtkActor> crosssectionActor = vtkSmartPointer<vtkActor>::New();
	crosssectionActor->SetMapper(crossPolyDataMapper);
	crosssectionActor->GetProperty()->SetDiffuseColor(0.5, 1, 0.7);

	//set the width of the plane(actor)
	crosssectionActor->GetProperty()->SetLineWidth(2);

	// -------------Setup cross section----------------

	//creating contour filter for 2-d contours
	vtkSmartPointer <vtkContourFilter> twod_iso = vtkSmartPointer <vtkContourFilter>::New();
	twod_iso->SetInputConnection(crossCutter->GetOutputPort());
	twod_iso->GenerateValues(50, 0, 50);

	// map the polygonal data to the graphic
	vtkSmartPointer<vtkPolyDataMapper> two_dDataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	two_dDataMapper->SetInputConnection(twod_iso->GetOutputPort());
	two_dDataMapper->ScalarVisibilityOff();


	// creating vtk actor for the 2d contour lines

	vtkSmartPointer<vtkActor> two_dActor = vtkSmartPointer<vtkActor>::New();
	two_dActor->SetMapper(two_dDataMapper);
	two_dActor->GetProperty()->SetColor(0, 0, 1);


	// ports declaration for three renderers

	double leftViewport[4] = { 0.0, 0.0, 0.33, 1.0 };
	double middleViewport[4] = { 0.33, 0.0, 0.66, 1.0 };
	double rightViewport[4] = { 0.66, 0.0, 1.0, 1.0 };

	//vtk renderers instantiating

	vtkSmartPointer<vtkRenderer> leftRenderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderer> middleRenderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderer> rightRenderer = vtkSmartPointer<vtkRenderer>::New();

	// vtk renderwindow instatiating

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(700, 700);

	// set viewports for the renderers
	leftRenderer->SetViewport(leftViewport);
	middleRenderer->SetViewport(middleViewport);
	rightRenderer->SetViewport(rightViewport);


	//Adding actors to their respective renderers

	leftRenderer->AddActor(isoActor);
	middleRenderer->AddActor(crosssectionActor);
	rightRenderer->AddActor(two_dActor);


	//adding renderers to the render window
	renderWindow->AddRenderer(leftRenderer);
	renderWindow->AddRenderer(middleRenderer);
	renderWindow->AddRenderer(rightRenderer);

	// instantiating render window interaction

	vtkSmartPointer<vtkRenderWindowInteractor> renderInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderInteractor->SetRenderWindow(renderWindow);

	//------------------slider setup for iso-values---------------------------
	vtkSmartPointer<vtkSliderRepresentation2D> sliderRepresentation = vtkSmartPointer<vtkSliderRepresentation2D>::New();
	sliderRepresentation->SetMinimumValue(2.0);
	sliderRepresentation->SetMaximumValue(128.0);
	sliderRepresentation->SetValue(50);
	sliderRepresentation->SetTitleText("iso value");
	//setting the co-ordinates for the iso-values slider
	sliderRepresentation->GetPoint1Coordinate()->SetCoordinateSystemToDisplay();
	sliderRepresentation->GetPoint1Coordinate()->SetValue(30, 70);
	sliderRepresentation->GetPoint2Coordinate()->SetCoordinateSystemToDisplay();
	sliderRepresentation->GetPoint2Coordinate()->SetValue(170, 70);

	//setting the slider Widget for iso-values slider
	vtkSmartPointer<vtkSliderWidget> sliderWidget = vtkSmartPointer<vtkSliderWidget>::New();
	sliderWidget->SetInteractor(renderInteractor);
	sliderWidget->SetRepresentation(sliderRepresentation);
	sliderWidget->SetAnimationModeToAnimate();
	sliderWidget->EnabledOn();

	// calling the iso-values slider callback function
	vtkSmartPointer<vtkSliderCallback> callback = vtkSmartPointer<vtkSliderCallback>::New();
	callback->sliderInteracter = isosurface_rep;
	callback->slidercutter = crossCutter;

	sliderWidget->AddObserver(vtkCommand::InteractionEvent, callback);

	//---------------------------slider setup for cross-section value----------------------------
	vtkSmartPointer<vtkSliderRepresentation2D> sliderRepresentation_cross = vtkSmartPointer<vtkSliderRepresentation2D>::New();
	sliderRepresentation_cross->SetMinimumValue(2.0);
	sliderRepresentation_cross->SetMaximumValue(55.0);
	sliderRepresentation_cross->SetValue(10.0);
	sliderRepresentation_cross->SetTitleText("cross section");

	//setting the co-ordinates for the cross-section slider
	sliderRepresentation_cross->GetPoint1Coordinate()->SetCoordinateSystemToDisplay();
	sliderRepresentation_cross->GetPoint1Coordinate()->SetValue(200, 70);
	sliderRepresentation_cross->GetPoint2Coordinate()->SetCoordinateSystemToDisplay();
	sliderRepresentation_cross->GetPoint2Coordinate()->SetValue(340, 70);

	//setting the slider Widget for cross-section slider
	vtkSmartPointer<vtkSliderWidget> sliderWidget_cross = vtkSmartPointer<vtkSliderWidget>::New();
	sliderWidget_cross->SetInteractor(renderInteractor);
	sliderWidget_cross->SetRepresentation(sliderRepresentation_cross);
	sliderWidget_cross->SetAnimationModeToAnimate();
	sliderWidget_cross->EnabledOn();

	// calling the cross-section slider callback function
	vtkSmartPointer<vtkSliderCallback_cross> callback_cross = vtkSmartPointer<vtkSliderCallback_cross>::New();
	callback_cross->sliderInteracter_cross = crossPlane;
	sliderWidget_cross->AddObserver(vtkCommand::InteractionEvent, callback_cross);


	//------------------set text to the renderers---------------------------
	vtkSmartPointer<vtkTextActor> text_iso_surface = vtkSmartPointer<vtkTextActor>::New();
	text_iso_surface->SetInput("Iso-Surfaces");
	text_iso_surface->SetPosition(50, 600);
	text_iso_surface->GetTextProperty()->SetFontSize(24);
	text_iso_surface->GetTextProperty()->SetColor(1.0, 0.0, 0.0);
	leftRenderer->AddActor2D(text_iso_surface);

	vtkSmartPointer<vtkTextActor> text_cross_section = vtkSmartPointer<vtkTextActor>::New();
	text_cross_section->SetInput("Cross Section");
	text_cross_section->SetPosition(50, 600);
	text_cross_section->GetTextProperty()->SetFontSize(24);
	text_cross_section->GetTextProperty()->SetColor(0, 1.0, 0.0);
	middleRenderer->AddActor2D(text_cross_section);

	vtkSmartPointer<vtkTextActor> text_contour_lines = vtkSmartPointer<vtkTextActor>::New();
	text_contour_lines->SetInput("2D Contour Lines");
	text_contour_lines->SetPosition(50, 600);
	text_contour_lines->GetTextProperty()->SetFontSize(24);
	text_contour_lines->GetTextProperty()->SetColor(0, 0.0, 1.0);
	rightRenderer->AddActor2D(text_contour_lines);


	//------Set background color for all the renderers
	leftRenderer->SetBackground(0.5, 0.7, 0.9);
	middleRenderer->SetBackground(0.9, 0.7, 0.6);
	rightRenderer->SetBackground(0.2, 0.5, 0.6);

	//setting camera for all the camera to have the same rotation and movement
	leftRenderer->ResetCamera();
	leftRenderer->GetActiveCamera();
	middleRenderer->ResetCamera();
	middleRenderer->SetActiveCamera(leftRenderer->GetActiveCamera());
	middleRenderer->GetActiveCamera();
	rightRenderer->ResetCamera();
	rightRenderer->SetActiveCamera(leftRenderer->GetActiveCamera());
	rightRenderer->GetActiveCamera();

	//render the renderers on the render window
	renderWindow->Render();

	//Start the graphic interaction with the actors and widget
	renderInteractor->Start();

}