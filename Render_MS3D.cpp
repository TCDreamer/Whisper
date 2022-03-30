

#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<gl\gl.h>
#include<gl\glu.h>
#include<time.h>

#include"ms3d.h"
#include"maths.h"


extern bool dbones;

void gotoxy(int x, int y) ;


// Renders the model!

void MS3D::
	Render(void)
	{
	
	int loopa;
	int loopb;
	int loopc;
int q=0;
	//Draw by group
//	glBindTexture(GL_TEXTURE_2D,texture[1]);
	
				

	for(loopa=0; loopa<numGroups; loopa++ )
		{
	/**/	
		int materialIndex = groups[loopa].materialIndex;

			glMaterialfv( GL_FRONT, GL_AMBIENT, materials[materialIndex].ambient );
			glMaterialfv( GL_FRONT, GL_DIFFUSE, materials[materialIndex].diffuse );
			glMaterialfv( GL_FRONT, GL_SPECULAR, materials[materialIndex].specular );
			glMaterialfv( GL_FRONT, GL_EMISSION, materials[materialIndex].emissive );
			glMaterialf( GL_FRONT, GL_SHININESS, materials[materialIndex].shininess );

				glBindTexture( GL_TEXTURE_2D, materials[materialIndex].texturedata);
/*	*/			glEnable( GL_TEXTURE_2D );


		
		//Draw as regular triangles, since .ms3d's aren't optimized like .md2's
	
	
		float vect[3];
		vect[0]=0;
		vect[1]=0;
		vect[2]=0;
		float nvct[3];
		nvct[0]=0;
		nvct[1]=0;
		nvct[2]=0;

		

		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_TRIANGLES);
			for(loopb=0; loopb<groups[loopa].numTriangles; loopb++)
				{
						
				int triangleIndex		=  groups[loopa].triangleIndices[loopb];
				const triangle* tri= &triangles[triangleIndex];

				//Loop through the triangle's vertices, and output them!
				for(loopc=0; loopc<3; loopc++)
					{
					int index= tri->vertexIndices[loopc];
					
					
					if(vertices[index].bone  == -1)
					{
					glNormal3fv( tri->vertexNormals[loopc]);
					glTexCoord2f(tri->s[loopc],1- tri->t[loopc]);
					glVertex3fv(vertices[index].vertex);
				
					}
					else
					{

					//rotates the normal vector for the animated triangles
					normrot(triangles[triangleIndex].vertexNormals[loopc], joints[vertices[index].boneID].matrf,nvct);
					glNormal3fv(nvct);
				
					glTexCoord2f(tri->s[loopc],1- tri->t[loopc]);
						
					//translates and rotates the vertices for each triangles to the correct location
					transvert(vertices[index].vertex, joints[vertices[index].bone].matrf,vect);

					
						glVertex3fv(vect);
					}

				
				
					}
				q++;
				}

		glEnd();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glLineWidth(5);
		//Draw the bones
	
		if(dbones==true)
		{
		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_LINES);	
		for(int x = 1; x < numJoints; x++)
		{
			//float * fMat = m_pJoints[x].m_matFinal.Get();
			//float * fMatParent = m_pJoints[m_pJoints[x].m_sParent].m_matFinal.Get();
			glVertex3f(joints[x].matrf[12], joints[x].matrf[13], joints[x].matrf[14]);
			glVertex3f(joints[joints[x].parent].matrf[12], joints[joints[x].parent].matrf[13], joints[joints[x].parent].matrf[14]);
		}
		glEnd();
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
			glVertex3f(1,1,0);
			glVertex3f(1,-1,0);
			glVertex3f(-1,-2,0);
			glVertex3f(-3,4,0);
		glEnd();
		}
//		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
///				}
		}

//	glEnable( GL_TEXTURE_2D );
	}





int MS3D::
	Animate(float speed, float starttime, float endtime, bool loop)
{
	static int first=1;
	static float tcheck= endtime;
//		int flag;

		if(first==1)
		{
			Reset();
			first=0;

		
		}
	
//		flag=0;
		static float lasttime= starttime;

		float fTime = time.getsec() * speed;

		
	
	
			
		fTime += lasttime;
		lasttime = fTime;

//		if(fTime> endtime-.03)//subtracted value to prevent the extra frames from being rendered. .03 because it is small but allows for extra room.
//		{
//	
//			flag=1;
//		}	
	
		
		//looping
		if(fTime > endtime || tcheck != endtime)
		{
			if(loop)
			{
				Reset();
				
				lasttime = starttime;
				fTime = starttime;
			//	tcheck = endtime;
			}
			else
				fTime = endtime;
		
	
		}	
				tcheck=endtime;
	
	
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

	for(int x = 0; x < numJoints; x++)
	{
		
		
		
		//Transformation matrix
		float mattmp[16];
		for(int k = 0;k<16; k++)
			mattmp[k]=0;
		//Current joint
		//Current frame
		unsigned int uiFrame = 0;

		//if there are no keyframes, don't do any transformations
		if(joints[x].numKeyFramesRot == 0 && joints[x].keyFramesTrans)//keyFramesTrans == 0)
		{

			Matrix_Copy(joints[x].matra,joints[x].matrf);
			continue;
		}





		//Calculate the current frame
		//Translation
		while(uiFrame < joints[x].numKeyFramesTrans && joints[x].keyFramesTrans[uiFrame].time < fTime)
			uiFrame++;
		joints[x].ctraframe = uiFrame;


				if(x==0)
		{
			gotoxy(5,uiFrame+10);
			printf("Frame: %i, %f",uiFrame, fTime);
		}
		float fTranslation[3];
		float fDeltaT = 1;
		float fInterp = 0;

		//If its at the extremes
		if(uiFrame == 0)
			memcpy(fTranslation,joints[x].keyFramesTrans[0].position, sizeof(float[3]));
		else if(uiFrame == joints[x].numKeyFramesTrans)
			memcpy(fTranslation, joints->keyFramesTrans[uiFrame-1].position, sizeof(float[3]));
		//If its in the middle of two frames
		else
		{
			
			fDeltaT = joints[x].keyFramesTrans[uiFrame].time - joints[x].keyFramesTrans[uiFrame-1].time;
			fInterp = (fTime - joints[x].keyFramesTrans[uiFrame-1].time) / fDeltaT;
			
			//Interpolate between the translations
			fTranslation[0] = joints[x].keyFramesTrans[uiFrame-1].position[0] + (joints[x].keyFramesTrans[uiFrame].position[0] - joints[x].keyFramesTrans[uiFrame-1].position[0]) * fInterp;
			fTranslation[1] = joints[x].keyFramesTrans[uiFrame-1].position[1] + (joints[x].keyFramesTrans[uiFrame].position[1] - joints[x].keyFramesTrans[uiFrame-1].position[1]) * fInterp;
			fTranslation[2] = joints[x].keyFramesTrans[uiFrame-1].position[2] + (joints[x].keyFramesTrans[uiFrame].position[2] - joints[x].keyFramesTrans[uiFrame-1].position[2]) * fInterp;
		}
		//Calculate the current rotation

		
		uiFrame = 0;
		while(uiFrame < joints[x].numKeyFramesRot && joints[x].keyFramesRot[uiFrame].time < fTime)

			uiFrame++;


		//If its at the extremes
		if(uiFrame == 0)
			Set_Rotation(joints[x].keyFramesRot[0].rotation[0],joints[x].keyFramesRot[0].rotation[1],joints[x].keyFramesRot[0].rotation[2],mattmp);
		
			
		else if(uiFrame == joints[x].numKeyFramesTrans)

			Set_Rotation(joints[x].keyFramesRot[uiFrame-1].rotation[0],joints[x].keyFramesRot[uiFrame-1].rotation[1],joints[x].keyFramesRot[uiFrame-1].rotation[2],mattmp);

		//If its in the middle of two frames, use a quaternion SLERP operation to calculate a new position
		else
		{

			fDeltaT = joints[x].keyFramesRot[uiFrame].time - joints[x].keyFramesRot[uiFrame-1].time;  // pkCur->m_fTime - pkPrev->m_fTime;
			fInterp = (fTime - joints[x].keyFramesRot[uiFrame-1].time) / fDeltaT;  //(fTime - pkPrev->m_fTime) / fDeltaT;

			//Create a rotation quaternion for each frame
			float cquat[4];
			float pquat[4];
			float fquat[4];
			
			getqeulers(joints[x].keyFramesRot[uiFrame].rotation,  cquat);
			getqeulers(joints[x].keyFramesRot[uiFrame-1].rotation,  pquat);
			//SLERP between the two frames
			
			SLERP(pquat,cquat, fInterp,fquat);
			//Convert the quaternion to a rotation matrix
			qmatrix(fquat,mattmp);
		}
		//Set the translation part of the matrix

		Set_Position(fTranslation[0],fTranslation[1], fTranslation[2], mattmp);

		//Calculate the joints final transformation


		float finmat[16];
		Matrix_Mult(joints[x].matrl, mattmp, finmat);
		
		//if there is no parent, just use the matrix you just made
		if(joints[x].parent == -1)
			
			Matrix_Copy(finmat, joints[x].matrf);

		//otherwise the final matrix is the parents final matrix * the new matrix
		else
		
		Matrix_Mult(joints[joints[x].parent].matrf, finmat,joints[x].matrf);



	}

Render();

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////






//return flag;
return 0;

}


void MS3D::
	Reset()
{
	

	
	time.init();
	time.getsec();
}