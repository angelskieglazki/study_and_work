//
// Created by max on 07.03.23.
//
#ifndef POLICIES_EX1_POLICY_H
#define POLICIES_EX1_POLICY_H




namespace Policies {
namespace Modifiers {

    struct NullModifier {
        template<typename T>
        static void Apply(T* other) {
            std::cout << "No modifier to apply" << '\n';
        }
    };

    struct FireModifier {
        template<typename T>
        static void Apply(T* other) {
            std::cout << other->name << " was burning" << '\n';
        }
    };

    struct PoisonModifier {
        template<typename T>
        static void Apply(T* other) {
            std::cout << other->name << " was poisoned" << '\n';
        }
    };
}

namespace Trajectories {

    struct NullTrajectory {
        static void Spawn() {
            std::cout << "Spawn in place" << '\n';
        }
    };

    struct ReverseTrajectory {
        static void Spawn() {
            std::cout << "Spawn in opposite direction" << '\n';
        }
    };

    struct SameTrajectory {
        static void Spawn() {
            std::cout << "Spawn in same direction" << '\n';
        }
    };
}

namespace Attacks {

    template< class ModifierPolicy, class TrajectoryPolicy >
    struct SpawnEntity : public ModifierPolicy, public TrajectoryPolicy {
        static void Spawn() {
            TrajectoryPolicy::Spawn();
        }
    };

    template< class ModifierPolicy, class TrajectoryPolicy >
    struct Collide : public ModifierPolicy, public TrajectoryPolicy {
        template<class T>
        static void Collided(T* other) {
            TrajectoryPolicy::Apply(other);
        }
    };
}
}


#endif //POLICIES_EX1_POLICY_H
