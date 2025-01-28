#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Margit {
	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		TILT_LEFT,
		TILT_RIGHT
	};

	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float ROLL = 0.0f;
	const float SPEED = 10.0f;
	const float SENSITIVITY = 0.05f;
	const float ZOOM = 45.0f;

	class Camera
	{
	public:
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		float Yaw;
		float Pitch;
		float Roll;

		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;

		// constructor with vectors
		Camera(
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw = YAW, float pitch = PITCH, float roll = ROLL)
			:
			Front(glm::vec3(0.0f, 0.0f, -1.0f)),
			MovementSpeed(SPEED),
			MouseSensitivity(SENSITIVITY),
			Zoom(ZOOM) {
			Position = position;
			WorldUp = up;
			Yaw = yaw;
			Pitch = pitch;
			Roll = roll;
			updateCameraVectors();
		}

		// constructor with scalar values
		Camera(
			float posX,
			float posY,
			float posZ,
			float upX,
			float upY,
			float upZ,
			float yaw,
			float pitch,
			float roll)
			:
			Front(glm::vec3(0.0f, 0.0f, -1.0)),
			MovementSpeed(SPEED),
			MouseSensitivity(SENSITIVITY),
			Zoom(ZOOM) {
			Position = glm::vec3(posX, posY, posZ);
			WorldUp = glm::vec3(upX, upY, upZ);
			Yaw = yaw;
			Pitch = pitch;
			Roll = roll;
			updateCameraVectors();
		}

		glm::mat4 GetViewMatrix() {
			return glm::lookAt(Position, Position + Front, Up);
		}

		glm::mat4 GetPerspectiveMatrix(float width, float height) {
			return glm::perspective(glm::radians(this->Zoom), width / height, 0.1f, 100.0f);
		}

		glm::mat4 GetOrthoMatrix(float width, float height, float scale = 1.0f) {
			float aspect = width / height;

			return glm::ortho(-aspect * scale, aspect * scale, -scale, scale, 0.1f, 100.0f);
		}

		void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
			float velocity = MovementSpeed * deltaTime;

			switch (direction) {
			case FORWARD:
				Position += glm::normalize(glm::vec3(Front.x, 0, Front.z)) * velocity;
				break;
			case BACKWARD:
				Position -= glm::normalize(glm::vec3(Front.x, 0, Front.z)) * velocity;
				break;
			case LEFT:
				Position -= Right * velocity;
				break;
			case RIGHT:
				Position += Right * velocity;
				break;
			case UP:
				Position += WorldUp * velocity;
				break;
			case DOWN:
				Position -= WorldUp * velocity;
				break;
			case TILT_LEFT:
				Roll -= velocity * 10.0f;
				break;
			case TILT_RIGHT:
				Roll += velocity * 10.0f;
				break;
			default:
				break;
			}

			updateCameraVectors();
		}

		void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;

			Yaw += xoffset;
			Pitch += yoffset;

			// make sure that when the pitch is out of bounds, screen doesn't get flipped
			if (constrainPitch) {
				if (Pitch > 89.0f)
					Pitch = 89.0f;
				if (Pitch < -89.0f)
					Pitch = -89.0f;
			}

			// update Front, Right and Up vectors using the updated Euler angles
			updateCameraVectors();
		}

		void ProcessMouseScroll(float yoffset) {
			Zoom -= (float)yoffset;
			if (Zoom < 1.0f)
				Zoom = 1.0f;
			if (Zoom > 45.0f)
				Zoom = 45.0f;
		}

	private:
		void updateCameraVectors() {
			// calculate the new front vector
			glm::vec3 front;
			front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			front.y = sin(glm::radians(Pitch));
			front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			Front = glm::normalize(front);

			// re-calculate the Right and Up vector
			Right = glm::normalize(glm::cross(Front, WorldUp)); // normalise the vectors, because their legnths gets closer to 0 the more you look up or down which results in slower movement
			Up = glm::normalize(glm::cross(Right, Front));

			// Apply roll
			Right = Right * cos(glm::radians(Roll)) + Up * sin(glm::radians(Roll));
			Up = glm::cross(Right, Front);
		}
	};
}