//
//  Framework for a raytracer
//  File: raytracer.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Author: Maarten Everts
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "raytracer.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

void operator >> (const YAML::Node& node, Triple& t)
{
    assert(node.size()==3);
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
}

Triple parseTriple(const YAML::Node& node)
{
    Triple t;
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;	
    return t;
}

Material* Raytracer::parseMaterial(const YAML::Node& node)
{
    Material *m = new Material();
    node["color"] >> m->color;	
    node["ka"] >> m->ka;
    node["kd"] >> m->kd;
    node["ks"] >> m->ks;
    node["n"] >> m->n;
    return m;
}


/*
* The program choose the type of object to render (implemented sphere, plane and triangle)
*/
Object* Raytracer::parseObject(const YAML::Node& node)
{
    Object *returnObject = NULL;
    std::string objectType;
    node["type"] >> objectType;

    if (objectType == "sphere") {
        Point pos;
        node["position"] >> pos;
        double r;
        node["radius"] >> r;
        Sphere *sphere = new Sphere(pos,r);		
        returnObject = sphere;
    } else if(objectType == "plane") {
	Point pos;
	node["position"] >>pos;
	Vector n;
	node["normal"] >> n;
	Plane *plane= new Plane(pos, n);
	returnObject=plane;
    } else if(objectType == "triangle") {
        Point v1, v2, v3;
        node["vertice1"] >>v1;
	node["vertice2"] >>v2;
	node["vertice3"] >>v3;
        Triangle *triangle =new Triangle(v1,v2,v3);
        returnObject=triangle;
    }

  

    if (returnObject) {
        // read the material and attach to object
        returnObject->material = parseMaterial(node["material"]);
    }

    return returnObject;
}

Light* Raytracer::parseLight(const YAML::Node& node)
{
    Point position;
    node["position"] >> position;
    Color color;
    node["color"] >> color;
    return new Light(position,color);
}

void Raytracer::parseSize(const YAML::Node& node)
{
   node[0] >> width;
   node[1] >> height;

}

int Raytracer::parseSS(const YAML::Node& node)
{
   int ss;
   node["position"] >> ss;
   return ss;
}

/*
* Read a scene from file
*/

bool Raytracer::readScene(const std::string& inputFilename)
{
    // Initialize a new scene
    scene = new Scene();

    // Open file stream for reading and have the YAML module parse it
    std::ifstream fin(inputFilename.c_str());
    if (!fin) {
        cerr << "Error: unable to open " << inputFilename << " for reading." << endl;;
        return false;
    }
    try {
        YAML::Parser parser(fin);
        if (parser) {
            YAML::Node doc;
            parser.GetNextDocument(doc);
	    
	    //Read the type of render mode needed
            if(doc.FindValue("RenderMode")) {
               std::string renderMode;
 	       doc["RenderMode"] >> renderMode;
	       scene->setRenderMode(renderMode);
	    }else{
   	       scene->setRenderMode("default");
            }

	    //Read the shadows of the figure if it has
	    if(doc.FindValue("Shadows"))  {
		bool shadows;
		doc["Shadows"] >> shadows;
		scene->setShadows(shadows);
	    }else{
		scene->setShadows(false);
	    }

	    //Read the number of reflections per object
	    if(doc.FindValue("MaxRecursionDepth")) {
		int recDepth;
		doc["MaxRecursionDepth"] >> recDepth;
		scene->setRecDepth(recDepth);
	    }

	    //Read the super-sampling values
	    if(const YAML::Node* sample = doc.FindValue("SuperSampling")) {
		int factor;
		(*sample)["factor"] >> factor;
                scene->setSSFactor(factor);
	    }else{
		scene->setSSFactor(1);
	    }

            // Read scene configuration options

	    if(const YAML::Node* sample = doc.FindValue("Camera")) {
		Triple eye;
		Point center;
		Vector up;
		
		(*sample)["eye"] >> eye;
		(*sample)["center"] >> center;
		(*sample)["up"] >> up;
		
		parseSize((*sample)["viewSize"]);

		Camera *camera= new Camera(eye,center,up, width, height);

		scene->setCamera(camera);


	    }else{
                scene->setEye(parseTriple(doc["Eye"]));
		width=400;		//default value
                height=400;		//default value
	    }
            // Read and parse the scene objects
            const YAML::Node& sceneObjects = doc["Objects"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of objects." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneObjects.begin();it!=sceneObjects.end();++it) {
                Object *obj = parseObject(*it);
                // Only add object if it is recognized
                if (obj) {
                    scene->addObject(obj);
                } else {
                    cerr << "Warning: found object of unknown type, ignored." << endl;
                }
            }

            // Read and parse light definitions
            const YAML::Node& sceneLights = doc["Lights"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of lights." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneLights.begin();it!=sceneLights.end();++it) {
                scene->addLight(parseLight(*it));
            }
        }
        if (parser) {
            cerr << "Warning: unexpected YAML document, ignored." << endl;
        }
    } catch(YAML::ParserException& e) {
        std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
        return false;
    }

    cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << endl;
    return true;
}

void Raytracer::renderToFile(const std::string& outputFilename)
{
    Image img(width,height);
    cout << "Tracing..." << endl;
    scene->render(img);
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;
}
