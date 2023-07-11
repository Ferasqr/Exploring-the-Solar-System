void PlanetArray(SolarSystem* A, float Ar, glm::mat4& ViewPrj, glm::mat4& World, float ObjSpeed) {
	// The procedure must implement the game logic  to move the character in third person.
	// Input:
	// <Assignment07 *A> Pointer to the current assignment code. Required to read the input from the user
	// <float Ar> Aspect ratio of the current window (for the Projection Matrix)
	// Output:
	// <glm::mat4 &ViewPrj> the view-projection matrix of the camera
	// <glm::mat4 &World> the world matrix of the object
	// Parameters
	// Camera FOV-y, Near Plane and Far Plane
	const float FOVy = glm::radians(40.0f);
	const float nearPlane = 0.0001f;
	const float farPlane = 100000.0f;





	// Player starting point
	//glm::vec3 StartingPosition = SPos;
	glm::vec3 StartingPosition = glm::vec3(0, 0, -12.0f);

	// Camera target height and distance
	const float camHeight = 0.0015f;
	const float camDist = 0.015f;
	// Camera Pitch limits
	//const float minPitch = glm::radians(-180.0f);
	//const float maxPitch = glm::radians(180.0f);
	// Rotation and motion speed
	const float ROT_SPEED = glm::radians(30.0f);
	const float MOVE_SPEED = ObjSpeed;
	// Camera Pitch limits
	const float minPitch = glm::radians(-8.75f);
	const float maxPitch = glm::radians(60.0f);
	// Integration with the timers and the controllers
	// returns:
	// <float deltaT> the time passed since the last frame
	// <glm::vec3 m> the state of the motion axes of the controllers (-1 <= m.x, m.y, m.z <= 1)
	// <glm::vec3 r> the state of the rotation axes of the controllers (-1 <= r.x, r.y, r.z <= 1)
	// <bool fire> if the user has pressed a fire button (not required in this assginment)
	float deltaT;
	glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
	bool fire = false;
	A->getSixAxis(deltaT, m, r, fire);

	// Game Logic implementation
// Current Player Position - statc variables make sure thattheri value remain unchanged in subsequent calls to the procedure
	glm::vec3 Up = glm::vec3(0, 1, 0);
	static glm::vec3 Pos = StartingPosition;
	static float yaw, pitch, roll;

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), yaw, glm::vec3(0, 1, 0));// Calculate rotation matrix

	// Calculate transformed direction vectors using rotationMatrix
	glm::vec3 ux = glm::vec3(rotationMatrix * glm::vec4(1, 0, 0, 1));
	glm::vec3 uy = glm::vec3(0, 1, 0);
	glm::vec3 uz = glm::vec3(rotationMatrix * glm::vec4(0, 0, -1, 1));

	pitch += ROT_SPEED * r.x * deltaT;
	pitch = glm::clamp(pitch, minPitch, maxPitch);// Update pitch, clamping within limits

	// Update yaw and roll angles
	yaw += ROT_SPEED * r.y * deltaT;
	roll += ROT_SPEED * r.z * deltaT;

	// Update position vector
	Pos += ux * MOVE_SPEED * m.x * deltaT;
	Pos += uy * MOVE_SPEED * m.y * deltaT;
	Pos += uz * MOVE_SPEED * m.z * deltaT;

	glm::mat4 rotationYaw = glm::rotate(glm::mat4(1), yaw, glm::vec3(0, 1, 0));
	glm::mat4 rotationPitch = glm::rotate(glm::mat4(1), pitch, glm::vec3(1, 0, 0));
	glm::mat4 rotationRoll = glm::rotate(glm::mat4(1), roll, glm::vec3(0, 0, 1));
	glm::mat4 Rot = rotationYaw * rotationPitch * rotationRoll; // Combine rotation matrices


	glm::mat4 Proj = glm::perspective(FOVy, Ar, nearPlane, farPlane); //perspective projection matrix
	Proj[1][1] *= -1;

	glm::vec3 c = glm::vec3(0, camHeight, camDist); //camera position vector
	glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(Rot * glm::vec4(c, 1)));
	glm::mat4 View = glm::lookAt(glm::vec3(translation * glm::vec4(Pos, 1)), Pos, Up); //view matrix
	ViewPrj = Proj * View;

	translation = glm::translate(glm::mat4(1), Pos);
	World = translation * Rot;

	//damping? 8 s43
	glm::mat4 Pnew = ViewPrj;
	glm::mat4 pOld = glm::mat4(1);
	float dampingspeed = 10;
	if (pOld == glm::mat4(1))
		pOld = ViewPrj;

	float factor = exp(-dampingspeed * deltaT);
	ViewPrj = pOld * factor + Pnew * (1 - factor), -1.0f, 1.0f;

	pOld = ViewPrj;
}

