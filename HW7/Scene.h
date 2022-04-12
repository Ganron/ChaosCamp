#pragma once
#include"Settings.h"
#include"Camera.h"
#include"Mesh.h"
#include<vector>
#include<string>

#include"rapidjson/document.h"

namespace ChaosCampAM {

  //A scene object holding all the data for a scene:
  // - Geometry
  // - Camera
  // - Scene settings
  // - (to be added) lighting, materials, etc.
  // Data is loaded from a .crtscene json file.
  //
  //This is a scene description only! All rendering functionality is in the dedicated Renderer class.
  class Scene {
  public: 
    //Extract a scene from a .crtscene json file
    Scene(const std::string& filename);

    //Getters
    const std::vector<Mesh>& getMeshes() const;
    const Camera& getCamera() const;
    const Settings& getSettings() const;

    //Setters
    void setCamera(const Camera& newCam);
    void setSettings(const Settings& newSettings);

  private:
    std::vector<Mesh> meshes;
    Camera cam;
    Settings settings;

    //Extract a rapidjson document from the scene file given
    rapidjson::Document getJsonDoc(const std::string& filename);

    //Extract scene settings from the rapidjson document
    void parseSettings(const rapidjson::Document& doc);

    //Extract scene camera from the rapidjson document
    void parseCamera(const rapidjson::Document& doc);

    //Extract object (meshes) form the rapidjson document
    void parseObjects(const rapidjson::Document& doc);

    //Convert a vector object (geometric 3D vector) from rapidjson array to a local Vector3 object.
    Vector3 loadVector(const rapidjson::Value::ConstArray& arr);

    //Convert a matrix object from rapidjson array to a local Matrix3x3 object.
    Matrix3x3 loadMatrix(const rapidjson::Value::ConstArray& arr);

    //Convert a colour tuple from rapidjson array to a local ColorRGB object.
    ColorRGB loadColorRGB(const rapidjson::Value::ConstArray& arr);

    //Convert a vertex list from rapidjson array to a local list of Vector3 objects.
    void loadVertices(const rapidjson::Value::ConstArray& arr, std::vector<Vector3>& vertices);

    //Convert a triangle list from rapidjson array to a local list of TriProxy objects.
    void loadTriangles(const rapidjson::Value::ConstArray& arr, std::vector<TriProxy>& triangles);

  };
}
