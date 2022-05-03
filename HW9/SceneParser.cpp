#include "SceneParser.h"
#include "Scene.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "Constants.h"

#include "rapidjson/istreamwrapper.h"

namespace ChaosCampAM {
  void SceneParser::parse(const std::string& filename, Scene& scene) {
    rapidjson::Document doc = getJsonDoc(filename);

    parseSettings(scene,doc);
    parseCamera(scene,doc);
    parseObjects(scene,doc);
    parseLights(scene, doc);
    parseMaterials(scene, doc);
  }

  rapidjson::Document SceneParser::getJsonDoc(const std::string& filename) {
    std::ifstream input(filename);
    assert(input.is_open());

    //Parse to rapidjson DOM
    rapidjson::IStreamWrapper inStream(input);
    rapidjson::Document doc;
    doc.ParseStream(inStream);

    if (doc.HasParseError()) {
      std::cout << "Parse Error: " << doc.GetParseError() << "\n";
      std::cout << "Error Offset: " << doc.GetErrorOffset() << "\n";
      assert(false);
    }

    assert(doc.IsObject());
    return doc;
  }

  void SceneParser::parseSettings(Scene& scene, const rapidjson::Document& doc) {
    const rapidjson::Value& settingsVal = doc.FindMember(STR_SETTINGS)->value;

    if (!settingsVal.IsNull() && settingsVal.IsObject()) {
      Settings settings;

      //Extract Background colour
      const rapidjson::Value& bgColVal = settingsVal.FindMember(STR_BG_COLOR)->value;
      assert(!bgColVal.IsNull() && bgColVal.IsArray());
      settings.setBgColor(loadVector(bgColVal.GetArray()));

      //Extract width and height
      const rapidjson::Value& imgSettingsVal = settingsVal.FindMember(STR_IMG_SETTINGS)->value;
      assert(!imgSettingsVal.IsNull() && imgSettingsVal.IsObject());
      const rapidjson::Value& imgWidthVal = imgSettingsVal.FindMember(STR_WIDTH)->value;
      const rapidjson::Value& imgHeightVal = imgSettingsVal.FindMember(STR_HEIGHT)->value;
      assert(!imgWidthVal.IsNull() && imgWidthVal.IsInt() && !imgHeightVal.IsNull() && imgHeightVal.IsInt());
      settings.setWidth(imgWidthVal.GetInt());
      settings.setHeight(imgHeightVal.GetInt());

      scene.setSettings(settings);
    }
  }

  void SceneParser::parseCamera(Scene& scene, const rapidjson::Document& doc) {
    const rapidjson::Value& camVal = doc.FindMember(STR_CAMERA)->value;
    if (!camVal.IsNull() && camVal.IsObject()) {
      Camera cam;

      //Extract position
      const rapidjson::Value& camPosVal = camVal.FindMember(STR_POS)->value;
      assert(!camPosVal.IsNull() && camPosVal.IsArray());
      cam.setPosition(loadVector(camPosVal.GetArray()));

      //Extract orientation
      const rapidjson::Value& camOrientVal = camVal.FindMember(STR_MATRIX)->value;
      assert(!camOrientVal.IsNull() && camOrientVal.IsArray());
      cam.setOrientation(loadMatrix(camOrientVal.GetArray()));

      scene.setCamera(cam);
    }
  }

  void SceneParser::parseLights(Scene& scene, const rapidjson::Document& doc) {
    const rapidjson::Value& lightVal = doc.FindMember(STR_LIGHTS)->value;
    if (!lightVal.IsNull() && lightVal.IsArray()) {
      scene.reservePointLights(lightVal.Size());

      for (int i = 0; i < lightVal.Size(); i++) { //For each light
        //Extract intensity
        const rapidjson::Value& intensityVal = lightVal[i].FindMember(STR_LIGHT_INTENSITY)->value;
        assert(!intensityVal.IsNull() && intensityVal.IsNumber());
        float intensity = intensityVal.GetFloat();

        //Extract position
        const rapidjson::Value& posVal = lightVal[i].FindMember(STR_POS)->value;
        assert(!posVal.IsNull() && posVal.IsArray());
        Vector3 position = loadVector(posVal.GetArray());

        scene.addPointLight(PointLight(position,intensity));
      }
    }
  }

  void SceneParser::parseMaterials(Scene& scene, const rapidjson::Document& doc) {
    const rapidjson::Value& matVal = doc.FindMember(STR_MATERIALS)->value;
    if (!matVal.IsNull() && matVal.IsArray()) {
      scene.reserveMaterials(matVal.Size());

      for (int i = 0; i < matVal.Size(); i++) { // For each material
        //Extract type
        const rapidjson::Value& typeVal = matVal[i].FindMember(STR_MAT_TYPE)->value;
        assert(!typeVal.IsNull() && typeVal.IsString());
        std::string typeStr = typeVal.GetString();
        MaterialType type = MaterialType::Diffuse; //Diffuse by default
        if (typeStr.compare(STR_MAT_TYPE_REFLECTIVE) == 0) { //Reflective
          type = MaterialType::Reflective;
        }

        //Extract albedo
        const rapidjson::Value& albedoVal = matVal[i].FindMember(STR_MAT_ALBEDO)->value;
        assert(!albedoVal.IsNull() && albedoVal.IsArray());
        Vector3 albedo = loadVector(albedoVal.GetArray());

        //Extract smooth_shading
        const rapidjson::Value& smoothVal = matVal[i].FindMember(STR_MAT_SMOOTH)->value;
        assert(!smoothVal.IsNull() && smoothVal.IsBool());
        bool smooth = smoothVal.GetBool();
        
        scene.addMaterial(Material(type, albedo, smooth));
      }
    }
  }

  void SceneParser::parseObjects(Scene& scene, const rapidjson::Document& doc) {
    const rapidjson::Value& objVal = doc.FindMember(STR_OBJECTS)->value;
    if (!objVal.IsNull() && objVal.IsArray()) {
      scene.reserveMeshes(objVal.Size());

      for (int i = 0; i < objVal.Size(); i++) { //For each mesh
        //Extract material index
        const rapidjson::Value& matIndexVal = objVal[i].FindMember(STR_MAT_INDEX)->value;
        assert(!matIndexVal.IsNull() && matIndexVal.IsInt());
        int matIndex = matIndexVal.GetInt();

        //Extract vertices
        const rapidjson::Value& verticesVal = objVal[i].FindMember(STR_VERTICES)->value;
        assert(!verticesVal.IsNull() && verticesVal.IsArray());
        std::vector<Vector3> vertices;
        loadVertices(verticesVal.GetArray(), vertices);

        //Extract triangles
        const rapidjson::Value& trianglesVal = objVal[i].FindMember(STR_TRIANGLES)->value;
        assert(!trianglesVal.IsNull() && trianglesVal.IsArray());
        std::vector<TriProxy> triangles;
        loadTriangles(trianglesVal.GetArray(), triangles);

        scene.addMesh(vertices, triangles, matIndex);
      }
    }
  }

  Vector3 SceneParser::loadVector(const rapidjson::Value::ConstArray& arr) {
    assert(arr.Size() == 3);
    Vector3 vec(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat());
    return vec;
  }

  Matrix3x3 SceneParser::loadMatrix(const rapidjson::Value::ConstArray& arr) {
    assert(arr.Size() == 9);
    //.crtscene stores matrices in column-major fashion!
    Matrix3x3 mat(
      arr[0].GetFloat(), arr[3].GetFloat(), arr[6].GetFloat(),//row 0
      arr[1].GetFloat(), arr[4].GetFloat(), arr[7].GetFloat(),//row 1
      arr[2].GetFloat(), arr[5].GetFloat(), arr[8].GetFloat() //row 2
    );
    return mat;
  }

  void SceneParser::loadVertices(const rapidjson::Value::ConstArray& arr, std::vector<Vector3>& vertices) {
    int arrSize = arr.Size();
    assert(arrSize % 3 == 0);

    //Empty vector and allocate required memory all at once
    vertices.clear();
    vertices.reserve(arrSize / 3);

    //Add all vertices to the list
    for (int i = 0; i < arrSize / 3; i++) {
      vertices.emplace_back(
        arr[3 * i].GetFloat(),
        arr[3 * i + 1].GetFloat(),
        arr[3 * i + 2].GetFloat()
      );
    }
  }

  void SceneParser::loadTriangles(const rapidjson::Value::ConstArray& arr, std::vector<TriProxy>& triangles) {
    int arrSize = arr.Size();
    assert(arrSize % 3 == 0);

    //Empty vector and allocate required memory all at once
    triangles.clear();
    triangles.reserve(arrSize / 3);

    for (int i = 0; i < arrSize / 3; i++) {
      triangles.emplace_back(
        arr[3 * i].GetInt(),
        arr[3 * i + 1].GetInt(),
        arr[3 * i + 2].GetInt()
      );
    }
  }
}
