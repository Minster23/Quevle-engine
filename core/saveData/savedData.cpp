#include <core/renderer/entity/objectEntity.hpp>
#include <nlohmann/json.hpp>
#include <core/saveData/savedData.hpp>
#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <core/model/model.hpp>
#include <core/renderer/renderer.hpp>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <chrono>

using namespace QuavleEngine;
using json = nlohmann::json;

extern ObjectEntity objectEntity;
extern Renderer renderer;

namespace {
    // Optimized conversion functions with bounds checking
    glm::vec3 toVec3(const json &j) {
        if (j.size() < 3) return glm::vec3(0.0f);
        return {j[0].get<float>(), j[1].get<float>(), j[2].get<float>()};
    }

    glm::vec4 toVec4(const json &j) {
        if (j.size() < 4) return glm::vec4(0.0f);
        return {j[0].get<float>(), j[1].get<float>(), j[2].get<float>(), j[3].get<float>()};
    }

    glm::mat4 toMat4(const json &j) {
        glm::mat4 mat(1.0f);
        if (j.size() >= 16) {
            const auto& jarr = j.get<std::vector<float>>();
            std::memcpy(glm::value_ptr(mat), jarr.data(), sizeof(float) * 16);
        }
        return mat;
    }

    // Batch operations for better performance
    struct LoadBatch {
        std::vector<std::string> modelPaths;
        std::vector<size_t> indices;
    };

    // Model cache to avoid reloading
    static std::unordered_map<std::string, bool> modelCache;

    bool isModelLoaded(const std::string& path) {
        return modelCache.find(path) != modelCache.end();
    }

    void markModelLoaded(const std::string& path) {
        modelCache[path] = true;
    }
}

void saveData::save() {
    auto start = std::chrono::high_resolution_clock::now();
    
    if (!std::filesystem::exists(locationManifest)) {
        createManifest();
    }
    
    if (saveObject()) {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Save completed successfully in " << duration.count() << "ms" << std::endl;
    }
}

void saveData::createManifest() {
    std::filesystem::create_directories(std::filesystem::path(locationManifest).parent_path());

    std::ofstream file(locationManifest);
    if (file) {
        file << "{}\n";
        std::cout << "Manifest file created at: " << locationManifest << std::endl;
    } else {
        std::cerr << "Failed to create manifest file.\n";
    }
}

bool saveData::saveObject() {
    json data;
    
    // Reserve space to avoid reallocations
    data["object"] = json::array();
    data["billboards"] = json::array();
    data["lights"] = json::array();
    data["cameras"] = json::array();

    // Pre-allocate arrays
    data["object"].get_ref<json::array_t&>().reserve(objectEntity.objects.size());
    data["billboards"].get_ref<json::array_t&>().reserve(objectEntity.billboards.size());
    data["lights"].get_ref<json::array_t&>().reserve(objectEntity.lights.size());
    data["cameras"].get_ref<json::array_t&>().reserve(cameras.size());

    // Save objects with optimized duplicate checking
    std::unordered_set<std::string> savedPaths;
    savedPaths.reserve(objectEntity.objects.size());

    for (const auto &obj : objectEntity.objects) {
        if (obj.locationMode.empty() || savedPaths.count(obj.locationMode)) {
            continue;
        }

        json jObj;
        jObj["modelPath"] = obj.locationMode;
        jObj["position"] = json::array({obj.position.x, obj.position.y, obj.position.z});
        jObj["rotation"] = json::array({obj.rotation.x, obj.rotation.y, obj.rotation.z});
        jObj["scale"] = json::array({obj.scale.x, obj.scale.y, obj.scale.z});
        jObj["isShow"] = obj.isShow;
        jObj["isSelected"] = obj.isSelected;
        jObj["name"] = obj.name;

        // Optimized material serialization
        const auto &mat = obj.material;
        jObj["material"] = json{
            {"ambient", json::array({mat.ambient.x, mat.ambient.y, mat.ambient.z})},
            {"diffuse", json::array({mat.diffuse.x, mat.diffuse.y, mat.diffuse.z})},
            {"specular", json::array({mat.specular.x, mat.specular.y, mat.specular.z})},
            {"emissive", json::array({mat.emissive.x, mat.emissive.y, mat.emissive.z})},
            {"shininess", mat.shininess},
            {"opacity", mat.opacity},
            {"shininessStrength", mat.shininessStrength}
        };

        data["object"].push_back(std::move(jObj));
        savedPaths.insert(obj.locationMode);
    }

    // Save billboards with optimized matrix conversion
    for (const auto &bb : objectEntity.billboards) {
        json jBillboard;
        jBillboard["name"] = bb.name;
        jBillboard["texLocation"] = bb.texLocation;
        jBillboard["vertices"] = bb.vertices;
        jBillboard["indices"] = bb.indices;
        jBillboard["indicesCount"] = bb.indicesCount;
        jBillboard["lookAt"] = bb.lookAt;
        jBillboard["isSelected"] = bb.isSelected;
        jBillboard["position"] = json::array({bb.position.x, bb.position.y, bb.position.z});
        jBillboard["rotation"] = json::array({bb.rotation.x, bb.rotation.y, bb.rotation.z});
        jBillboard["scale"] = json::array({bb.scale.x, bb.scale.y, bb.scale.z});
        jBillboard["color"] = json::array({bb.color.r, bb.color.g, bb.color.b, bb.color.a});
        jBillboard["isShow"] = bb.isShow;

        // Optimized matrix conversion
        const float* modelFlat = glm::value_ptr(bb.model);
        const float* viewFlat = glm::value_ptr(bb.view);
        const float* projFlat = glm::value_ptr(bb.projection);
        
        jBillboard["model"] = std::vector<float>(modelFlat, modelFlat + 16);
        jBillboard["view"] = std::vector<float>(viewFlat, viewFlat + 16);
        jBillboard["projection"] = std::vector<float>(projFlat, projFlat + 16);

        data["billboards"].push_back(std::move(jBillboard));
    }

    // Save lights (optimized)
    for (const auto &light : objectEntity.lights) {
        json jLight;
        jLight["position"] = json::array({light.position.x, light.position.y, light.position.z});
        jLight["rotation"] = json::array({light.rotation.x, light.rotation.y, light.rotation.z});
        jLight["scale"] = json::array({light.scale.x, light.scale.y, light.scale.z});
        jLight["lightColor"] = json::array({light.lightColor.r, light.lightColor.g, light.lightColor.b});
        jLight["name"] = light.name;
        jLight["intensity"] = light.intensity;
        jLight["isShow"] = light.isShow;
        jLight["textureID"] = light.textureID;
        jLight["components"] = light.components;

        data["lights"].push_back(std::move(jLight));
    }

    // Save cameras (optimized)
    for (const auto &camera : cameras) {
        if (camera.name.empty() || camera.type == CAMERATYPE::ENGINE) {
            data["cameras"].push_back(nullptr);
            continue;
        }

        json jCamera;
        jCamera["name"] = camera.name;
        jCamera["position"] = json::array({camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z});
        jCamera["cameraFront"] = json::array({camera.cameraFront.x, camera.cameraFront.y, camera.cameraFront.z});
        jCamera["cameraUp"] = json::array({camera.cameraUp.x, camera.cameraUp.y, camera.cameraUp.z});
        jCamera["rotation"] = json::array({camera.rotation.x, camera.rotation.y, camera.rotation.z});
        jCamera["scale"] = json::array({camera.scale.x, camera.scale.y, camera.scale.z});
        jCamera["vertexShader"] = camera.vertexShader;
        jCamera["fragmentShader"] = camera.fragmentShader;
        jCamera["shaderProgram"] = camera.shaderProgram;
        jCamera["VAO"] = camera.VAO;
        jCamera["VBO"] = camera.VBO;
        jCamera["textureID"] = camera.textureID;
        jCamera["deltaTime"] = camera.deltaTime;
        jCamera["lastFrame"] = camera.lastFrame;
        jCamera["firstMouse"] = camera.firstMouse;
        jCamera["yaw"] = camera.yaw;
        jCamera["pitch"] = camera.pitch;
        jCamera["lastX"] = camera.lastX;
        jCamera["lastY"] = camera.lastY;
        jCamera["fov"] = camera.fov;
        jCamera["worldUp"] = json::array({camera.worldUp.x, camera.worldUp.y, camera.worldUp.z});
        jCamera["isSelected"] = camera.isSelected;
        jCamera["isShow"] = camera.isShow;
        jCamera["type"] = static_cast<int>(camera.type);

        // Optimized matrix conversion
        const float* projFlat = glm::value_ptr(camera.projection);
        const float* viewFlat = glm::value_ptr(camera.view);
        const float* modelFlat = glm::value_ptr(camera.model);

        jCamera["projection"] = std::vector<float>(projFlat, projFlat + 16);
        jCamera["view"] = std::vector<float>(viewFlat, viewFlat + 16);
        jCamera["model"] = std::vector<float>(modelFlat, modelFlat + 16);

        data["cameras"].push_back(std::move(jCamera));
    }

    // Write to file with optimized settings
    std::ofstream outFile(locationManifest);
    if (outFile) {
        outFile << data.dump(4);
        outFile.flush();
        return true;
    }
    
    std::cerr << "Failed to write to " << locationManifest << std::endl;
    return false;
}

void saveData::load() {
    static bool alreadyLoaded = false;
    if (alreadyLoaded) {
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    if (!std::filesystem::exists(locationManifest)) {
        createManifest();
        alreadyLoaded = true;
        return;
    }

    std::ifstream inFile(locationManifest);
    if (!inFile) {
        std::cerr << "Error: Could not open save file: " << locationManifest << std::endl;
        return;
    }

    if (inFile.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "Save file is empty." << std::endl;
        alreadyLoaded = true;
        return;
    }

    json data;
    try {
        inFile >> data;
    } catch (const json::parse_error &e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return;
    }

    // Clear existing data with reserved capacity
    objectEntity.objects.clear();
    objectEntity.billboards.clear();
    objectEntity.lights.clear();
    cameras.clear();
    initCamera();

    // Pre-allocate vectors based on JSON data size
    if (data.contains("object") && data["object"].is_array()) {
        objectEntity.objects.reserve(data["object"].size());
    }
    if (data.contains("billboards") && data["billboards"].is_array()) {
        objectEntity.billboards.reserve(data["billboards"].size());
    }
    if (data.contains("lights") && data["lights"].is_array()) {
        objectEntity.lights.reserve(data["lights"].size());
    }
    if (data.contains("cameras") && data["cameras"].is_array()) {
        cameras.reserve(data["cameras"].size());
    }

    // Load objects with improved model loading
    if (data.contains("object") && data["object"].is_array()) {
        std::unordered_set<std::string> loadedPaths;
        loadedPaths.reserve(data["object"].size());

        // First pass: load unique models sequentially (safer than async)
        std::vector<std::string> uniqueModelPaths;
        for (const auto &jObj : data["object"]) {
            if (!jObj.contains("modelPath") || jObj["modelPath"].empty()) {
                continue;
            }
            
            std::string modelPath = jObj["modelPath"];
            if (loadedPaths.find(modelPath) == loadedPaths.end()) {
                uniqueModelPaths.push_back(modelPath);
                loadedPaths.insert(modelPath);
            }
        }

        // Load models one by one to avoid threading issues
        for (const auto& modelPath : uniqueModelPaths) {
            if (!isModelLoaded(modelPath)) {
                std::cout << "Loading model: " << modelPath << std::endl;
                if (renderer.loadModelFirst(modelPath)) {
                    markModelLoaded(modelPath);
                    std::cout << "Successfully loaded: " << modelPath << std::endl;
                } else {
                    std::cerr << "Failed to load model: " << modelPath << std::endl;
                }
            }
        }

        // Second pass: create object data
        loadedPaths.clear();
        for (const auto &jObj : data["object"]) {
            try {
                if (!jObj.contains("modelPath") || jObj["modelPath"].empty()) {
                    continue;
                }

                std::string modelPath = jObj["modelPath"];
                if (loadedPaths.count(modelPath)) {
                    continue;
                }

                // Only create object if model was loaded successfully
                if (!isModelLoaded(modelPath)) {
                    std::cerr << "Skipping object with failed model: " << modelPath << std::endl;
                    continue;
                }

                ObjectEntity::ObjectData obj;
                obj.locationMode = modelPath;

                // Safe property loading with null checks
                if (jObj.contains("position") && jObj["position"].is_array() && jObj["position"].size() >= 3) {
                    obj.position = toVec3(jObj["position"]);
                }
                if (jObj.contains("rotation") && jObj["rotation"].is_array() && jObj["rotation"].size() >= 3) {
                    obj.rotation = toVec3(jObj["rotation"]);
                }
                if (jObj.contains("scale") && jObj["scale"].is_array() && jObj["scale"].size() >= 3) {
                    obj.scale = toVec3(jObj["scale"]);
                }
                if (jObj.contains("isShow") && jObj["isShow"].is_boolean()) {
                    obj.isShow = jObj["isShow"].get<bool>();
                }
                if (jObj.contains("isSelected") && jObj["isSelected"].is_boolean()) {
                    obj.isSelected = jObj["isSelected"].get<bool>();
                }
                if (jObj.contains("name") && jObj["name"].is_string()) {
                    obj.name = jObj["name"].get<std::string>();
                }

                // Safe material loading
                if (jObj.contains("material") && jObj["material"].is_object()) {
                    const auto &jMat = jObj["material"];
                    if (jMat.contains("ambient") && jMat["ambient"].is_array()) {
                        obj.material.ambient = toVec3(jMat["ambient"]);
                    }
                    if (jMat.contains("diffuse") && jMat["diffuse"].is_array()) {
                        obj.material.diffuse = toVec3(jMat["diffuse"]);
                    }
                    if (jMat.contains("specular") && jMat["specular"].is_array()) {
                        obj.material.specular = toVec3(jMat["specular"]);
                    }
                    if (jMat.contains("emissive") && jMat["emissive"].is_array()) {
                        obj.material.emissive = toVec3(jMat["emissive"]);
                    }
                    if (jMat.contains("shininess") && jMat["shininess"].is_number()) {
                        obj.material.shininess = jMat["shininess"].get<float>();
                    }
                    if (jMat.contains("opacity") && jMat["opacity"].is_number()) {
                        obj.material.opacity = jMat["opacity"].get<float>();
                    }
                    if (jMat.contains("shininessStrength") && jMat["shininessStrength"].is_number()) {
                        obj.material.shininessStrength = jMat["shininessStrength"].get<float>();
                    }
                }

                objectEntity.objects.push_back(std::move(obj));
                loadedPaths.insert(modelPath);
            } catch (const std::exception &e) {
                std::cerr << "Error loading object: " << e.what() << std::endl;
            }
        }
    }

    // Load billboards with safety checks
    if (data.contains("billboards") && data["billboards"].is_array()) {
        for (const auto &jbb : data["billboards"]) {
            try {
                ObjectEntity::Billboard bb;

                if (jbb.contains("name") && jbb["name"].is_string()) {
                    bb.name = jbb["name"].get<std::string>();
                }
                if (jbb.contains("texLocation") && jbb["texLocation"].is_string()) {
                    bb.texLocation = jbb["texLocation"].get<std::string>();
                }
                if (jbb.contains("vertices") && jbb["vertices"].is_array()) {
                    bb.vertices = jbb["vertices"].get<std::vector<float>>();
                }
                if (jbb.contains("indices") && jbb["indices"].is_array()) {
                    bb.indices = jbb["indices"].get<std::vector<unsigned int>>();
                }
                if (jbb.contains("indicesCount") && jbb["indicesCount"].is_number()) {
                    bb.indicesCount = jbb["indicesCount"].get<int>();
                }
                if (jbb.contains("lookAt") && jbb["lookAt"].is_boolean()) {
                    bb.lookAt = jbb["lookAt"].get<bool>();
                }
                if (jbb.contains("isSelected") && jbb["isSelected"].is_boolean()) {
                    bb.isSelected = jbb["isSelected"].get<bool>();
                }
                if (jbb.contains("position") && jbb["position"].is_array()) {
                    bb.position = toVec3(jbb["position"]);
                }
                if (jbb.contains("rotation") && jbb["rotation"].is_array()) {
                    bb.rotation = toVec3(jbb["rotation"]);
                }
                if (jbb.contains("scale") && jbb["scale"].is_array()) {
                    bb.scale = toVec3(jbb["scale"]);
                }
                if (jbb.contains("color") && jbb["color"].is_array()) {
                    bb.color = toVec4(jbb["color"]);
                }
                if (jbb.contains("isShow") && jbb["isShow"].is_boolean()) {
                    bb.isShow = jbb["isShow"].get<bool>();
                }

                // Safe matrix loading
                if (jbb.contains("model") && jbb["model"].is_array()) {
                    bb.model = toMat4(jbb["model"]);
                }
                if (jbb.contains("view") && jbb["view"].is_array()) {
                    bb.view = toMat4(jbb["view"]);
                }
                if (jbb.contains("projection") && jbb["projection"].is_array()) {
                    bb.projection = toMat4(jbb["projection"]);
                }

                objectEntity.billboards.push_back(std::move(bb));
                
                // Load billboard resources safely
                try {
                    size_t index = objectEntity.billboards.size() - 1;
                    renderer.shaderLoader(index, Renderer::RenderType::BILLBOARD);
                    if (!bb.texLocation.empty()) {
                        renderer.loadTexture(bb.texLocation, index, Renderer::TextureType::BILLBOARD);
                    }
                    renderer.shaderLink(index, Renderer::RenderType::BILLBOARD);
                } catch (const std::exception &e) {
                    std::cerr << "Error loading billboard resources: " << e.what() << std::endl;
                }
            } catch (const std::exception &e) {
                std::cerr << "Error loading billboard: " << e.what() << std::endl;
            }
        }
    }

    // Load lights with safety checks
    if (data.contains("lights") && data["lights"].is_array()) {
        for (const auto &jlight : data["lights"]) {
            try {
                ObjectEntity::LightData light;

                if (jlight.contains("position") && jlight["position"].is_array()) {
                    light.position = toVec3(jlight["position"]);
                }
                if (jlight.contains("rotation") && jlight["rotation"].is_array()) {
                    light.rotation = toVec3(jlight["rotation"]);
                }
                if (jlight.contains("scale") && jlight["scale"].is_array()) {
                    light.scale = toVec3(jlight["scale"]);
                }
                if (jlight.contains("lightColor") && jlight["lightColor"].is_array()) {
                    light.lightColor = toVec3(jlight["lightColor"]);
                }
                if (jlight.contains("name") && jlight["name"].is_string()) {
                    light.name = jlight["name"].get<std::string>();
                }
                if (jlight.contains("intensity") && jlight["intensity"].is_number()) {
                    light.intensity = jlight["intensity"].get<float>();
                }
                if (jlight.contains("isShow") && jlight["isShow"].is_boolean()) {
                    light.isShow = jlight["isShow"].get<bool>();
                }
                if (jlight.contains("isSelected") && jlight["isSelected"].is_boolean()) {
                    light.isSelected = jlight["isSelected"].get<bool>();
                }
                if (jlight.contains("textureID") && jlight["textureID"].is_number()) {
                    light.textureID = jlight["textureID"].get<unsigned int>();
                }
                if (jlight.contains("components") && jlight["components"].is_array()) {
                    light.components = jlight["components"].get<std::vector<int>>();
                }

                objectEntity.lights.push_back(std::move(light));
                
                // Load light resources safely
                try {
                    if (light.textureID != 0) {
                        renderer.LightShaderLink();
                    }
                } catch (const std::exception &e) {
                    std::cerr << "Error loading light resources: " << e.what() << std::endl;
                }
            } catch (const std::exception &e) {
                std::cerr << "Error loading light: " << e.what() << std::endl;
            }
        }
    }

    // Load cameras (optimized)
    if (data.contains("cameras") && data["cameras"].is_array()) {
        for (size_t i = 1; i < data["cameras"].size(); i++) {
            const auto &cameraData = data["cameras"][i];
            if (cameraData.is_null()) continue;
            
            try {
                Camera cam;

                if (cameraData.contains("name")) cam.name = cameraData["name"].get<std::string>();
                if (cameraData.contains("position")) cam.cameraPos = toVec3(cameraData["position"]);
                if (cameraData.contains("cameraFront")) cam.cameraFront = toVec3(cameraData["cameraFront"]);
                if (cameraData.contains("cameraUp")) cam.cameraUp = toVec3(cameraData["cameraUp"]);
                if (cameraData.contains("rotation")) cam.rotation = toVec3(cameraData["rotation"]);
                if (cameraData.contains("scale")) cam.scale = toVec3(cameraData["scale"]);
                if (cameraData.contains("vertexShader")) cam.vertexShader = cameraData["vertexShader"].get<unsigned int>();
                if (cameraData.contains("fragmentShader")) cam.fragmentShader = cameraData["fragmentShader"].get<unsigned int>();
                if (cameraData.contains("shaderProgram")) cam.shaderProgram = cameraData["shaderProgram"].get<unsigned int>();
                if (cameraData.contains("VAO")) cam.VAO = cameraData["VAO"].get<unsigned int>();
                if (cameraData.contains("VBO")) cam.VBO = cameraData["VBO"].get<unsigned int>();
                if (cameraData.contains("textureID")) cam.textureID = cameraData["textureID"].get<unsigned int>();
                if (cameraData.contains("deltaTime")) cam.deltaTime = cameraData["deltaTime"].get<float>();
                if (cameraData.contains("lastFrame")) cam.lastFrame = cameraData["lastFrame"].get<float>();
                if (cameraData.contains("firstMouse")) cam.firstMouse = cameraData["firstMouse"].get<bool>();
                if (cameraData.contains("yaw")) cam.yaw = cameraData["yaw"].get<float>();
                if (cameraData.contains("pitch")) cam.pitch = cameraData["pitch"].get<float>();
                if (cameraData.contains("lastX")) cam.lastX = cameraData["lastX"].get<float>();
                if (cameraData.contains("lastY")) cam.lastY = cameraData["lastY"].get<float>();
                if (cameraData.contains("fov")) cam.fov = cameraData["fov"].get<float>();
                if (cameraData.contains("worldUp")) cam.worldUp = toVec3(cameraData["worldUp"]);
                if (cameraData.contains("isSelected")) cam.isSelected = cameraData["isSelected"].get<bool>();
                if (cameraData.contains("isShow")) cam.isShow = cameraData["isShow"].get<bool>();
                if (cameraData.contains("type")) {
                    int type = cameraData["type"].get<int>();
                    cam.type = (type == 1) ? CAMERATYPE::MASTER : CAMERATYPE::NONE;
                }

                // Optimized matrix loading
                if (cameraData.contains("projection")) cam.projection = toMat4(cameraData["projection"]);
                if (cameraData.contains("view")) cam.view = toMat4(cameraData["view"]);
                if (cameraData.contains("model")) cam.model = toMat4(cameraData["model"]);

                cameras.push_back(std::move(cam));
            } catch (const std::exception &e) {
                std::cerr << "Error loading camera: " << e.what() << std::endl;
            }
        }
    }

    alreadyLoaded = true;
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Loaded scene in " << duration.count() << "ms: " 
              << objectEntity.objects.size() << " objects, "
              << objectEntity.billboards.size() << " billboards, "
              << objectEntity.lights.size() << " lights" << std::endl;
}