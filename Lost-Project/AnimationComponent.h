#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Animation.h"
#include <string>
#include <unordered_map>

struct AnimationTransition {
    std::string from;
    std::string to;
    std::string param;
    bool expectedValue;
};

struct AnimationNode {
    std::string name = "";
    Animation* animation = nullptr;
    std::vector<AnimationTransition> transitions = {};
};

struct AnimatorParameter {
    enum class Type { Bool, Float, Int, Trigger };
    Type type;

    bool  boolValue = false;
    float floatValue = 0.0f;
    int   intValue = 0;
    bool  triggerFired = false; 
};

struct AnimationComponent {
    //std::unordered_map<std::string, AnimationNode*> nodes;
    std::unordered_map<std::string, AnimationNode> nodes;
    std::unordered_map<std::string, AnimatorParameter> parameters;
    std::string currentNode = "";
    float      currentTime = 0.0f;
    std::vector<glm::mat4> finalBoneMatrices;

    AnimationComponent() {
        finalBoneMatrices.resize(100, glm::mat4(1.0f));
    }

    void Add(const std::string& name, Animation* animation) {
        AnimationNode node;
        node.name = name;           
        node.animation = animation;

        nodes[name] = node;
        if (currentNode.empty()) currentNode = name;
    }

    void AddBool(const std::string& name, bool  val = false) {
        parameters[name] = { AnimatorParameter::Type::Bool,  val };
    }
    //void AddFloat(const std::string& name, float val = 0.0f) {
    //    parameters[name] = { AnimatorParameter::Type::Float, .floatValue = val };
    //}
    //void AddTrigger(const std::string& name) {
    //    parameters[name] = { AnimatorParameter::Type::Trigger };
    //}

    void SetBool(const std::string& name, bool  val) { parameters[name].boolValue = val; }

    //void SetFloat(const std::string& name, float val) { parameters[name].floatValue = val; }
    //void SetInt(const std::string& name, int   val) { parameters[name].intValue = val; }
    //void SetTrigger(const std::string& name) { parameters[name].triggerFired = true; }

    //void SetTrigger(const std::string& from, const std::string& to,
    //    std::function<bool()> condition) {
    //    if (!nodes.count(from)) return;
    //    //nodes[from].transitions.push_back({ from, to, condition });
    //}

    void SetTrigger(const std::string& from, const std::string& to,
        const std::string& paramName, bool expectedValue) {
        if (!nodes.count(from)) return;
        AnimationTransition t;
        t.from = from;
        t.to = to;
        t.param = paramName;
        t.expectedValue = expectedValue;
        auto& transition = nodes[from].transitions;
        transition.push_back(t);
    }

    Animation* getCurrentAnimation() {
        if (currentNode == "") return nullptr;
        return nodes[currentNode].animation;
    }

    void checkTransitions() {
        AnimationNode node = nodes[currentNode];
        //if (!node) return;

        for (auto& t : node.transitions) {
            if (!parameters.count(t.param)) continue;
            AnimatorParameter& param = parameters[t.param];

            bool conditionMet = false;

            switch (param.type) {
            case AnimatorParameter::Type::Bool:
                conditionMet = (param.boolValue == t.expectedValue);
                break;
            //case AnimatorParameter::Type::Float:
            //    conditionMet = (param.floatValue >= t.expectedValue);
            //    break;
            //case AnimatorParameter::Type::Trigger:
            //    conditionMet = param.triggerFired;
            //    if (conditionMet) param.triggerFired = false;  // reset after use
            //    break;
            }

            if (conditionMet) {
                currentNode = t.to;
                currentTime = 0.0f;
                return;
            }
        }
    }
};
