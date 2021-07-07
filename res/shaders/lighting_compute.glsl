#version 430
#extension GL_ARB_compute_shader :               enable
#extension GL_ARB_shader_storage_buffer_object : enable









#ifndef RENDER_DISTANCE
#define RENDER_DISTANCE 20
#endif
#ifndef VERTICAL_SIZE
#define VERTICAL_SIZE 16
#endif
#define XZ_BLOCKS RENDER_DISTANCE*16
#define Y_BLOCKS VERTICAL_SIZE*16

struct LightSource{
    int x, y, z;
    uint lightLevel;
};

layout(std430, binding = 1) buffer lightLevels{
    int levels[];
};

layout(std430, binding = 2) readonly buffer lightSources{
    LightSource lights[];
};

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

int getFromNeighbours(uint x, uint y, uint z){
    int levelPosY = (y+1)*Y_BLOCKS*Y_BLOCKS+(z)*XZ_BLOCKS+(x) < 0 ? 0 : levels[(y+1)*Y_BLOCKS*Y_BLOCKS+(z)*XZ_BLOCKS+(x)];
    int levelNegY = (y-1)*Y_BLOCKS*Y_BLOCKS+(z)*XZ_BLOCKS+(x) < 0 ? 0 : levels[(y-1)*Y_BLOCKS*Y_BLOCKS+(z)*XZ_BLOCKS+(x)];
    int levelPosX = (y)*Y_BLOCKS*Y_BLOCKS+(z)*XZ_BLOCKS+(x+1) < 0 ? 0 : levels[(y)*Y_BLOCKS*Y_BLOCKS+(z)*XZ_BLOCKS+(x+1)];
    int levelNegX = (y)*Y_BLOCKS*Y_BLOCKS+(z)*XZ_BLOCKS+(x-1) < 0 ? 0 : levels[(y)*Y_BLOCKS*Y_BLOCKS+(z)*XZ_BLOCKS+(x-1)];
    int levelPosZ = (y)*Y_BLOCKS*Y_BLOCKS+(z+1)*XZ_BLOCKS+(x) < 0 ? 0 : levels[(y)*Y_BLOCKS*Y_BLOCKS+(z+1)*XZ_BLOCKS+(x)];
    int levelNegZ = (y)*Y_BLOCKS*Y_BLOCKS+(z-1)*XZ_BLOCKS+(x) < 0 ? 0 : levels[(y)*Y_BLOCKS*Y_BLOCKS+(z-1)*XZ_BLOCKS+(x)];

    int newLevel = max(max(levelPosX, max(levelPosY, max(levelPosZ, max(levelNegX, max(levelNegY, levelNegZ)))))-1, 0);
    return newLevel;
//    return levelNegY;
}

void set(LightSource lightSource, uint x, uint y, uint z){
    if(x==0 && y==0 && z==0)return;
    if((lightSource.y+y)*Y_BLOCKS*Y_BLOCKS+(lightSource.z+z)*XZ_BLOCKS+(lightSource.x+x)<0)return;
    if(levels[(lightSource.y+y)*Y_BLOCKS*Y_BLOCKS+(lightSource.z+z)*XZ_BLOCKS+(lightSource.x+x)]==-1)return;

    levels[(lightSource.y+y)*Y_BLOCKS*Y_BLOCKS+(lightSource.z+z)*XZ_BLOCKS+(lightSource.x+x)] = max(getFromNeighbours(lightSource.x+x, lightSource.y+y, lightSource.z+z), levels[(lightSource.y+y)*16*16+(lightSource.z+z)*XZ_BLOCKS+(lightSource.x+x)]);

//    uint l = levels[(lightSource.y+y)*16*16+(lightSource.z+z)*16+(lightSource.x+x)];

//    levels[(lightSource.y+y)*16*16+(lightSource.z+z)*16+(lightSource.x+x)] = uint(l == -1) * l + uint(l != -1) * max(getFromNeighbours(lightSource.x+x, lightSource.y+y, lightSource.z+z), levels[(lightSource.y+y)*16*16+(lightSource.z+z)*16+(lightSource.x+x)]);

}


void main() {
    uint gid = gl_GlobalInvocationID.x;
    LightSource lightSource = lights[gid];

    levels[lightSource.y*Y_BLOCKS*Y_BLOCKS+lightSource.z*XZ_BLOCKS+lightSource.x] = int(lightSource.lightLevel);

    //Positive Y
    //Positive X
    for(int z = 0;z<lightSource.lightLevel;z++){
        for(int x = 0;x<max(lightSource.lightLevel-z, 0);x++){
            for(int y = 0;y<max(lightSource.lightLevel-x-z, 0);y++){
                set(lightSource, x, y, z);
            }
        }
    }
    for(int z = 0;z<lightSource.lightLevel;z++){
        for(int x = 0;x<max(lightSource.lightLevel-z, 0);x++){
            for(int y = 0;y<max(lightSource.lightLevel-x-z, 0);y++){
                set(lightSource, x, y, -z);
            }
        }
    }

    //Negative X
    for(int z = 0;z<lightSource.lightLevel;z++){
        for(int x = 0;x<max(lightSource.lightLevel-z, 0);x++){
            for(int y = 0;y<max(lightSource.lightLevel-x-z, 0);y++){
                set(lightSource, -x, y, z);
            }
        }
    }
    for(int z = 0;z<lightSource.lightLevel;z++){
        for(int x = 0;x<max(lightSource.lightLevel-z, 0);x++){
            for(int y = 0;y<max(lightSource.lightLevel-x-z, 0);y++){
                set(lightSource, -x, y, -z);
            }
        }
    }

    //Negative Y
    //Positive X
    for(int z = 0;z<lightSource.lightLevel;z++){
        for(int x = 0;x<max(lightSource.lightLevel-z, 0);x++){
            for(int y = 0;y<max(lightSource.lightLevel-x-z, 0);y++){
                set(lightSource, x, -y, z);
            }
        }
    }
    for(int z = 0;z<lightSource.lightLevel;z++){
        for(int x = 0;x<max(lightSource.lightLevel-z, 0);x++){
            for(int y = 0;y<max(lightSource.lightLevel-x-z, 0);y++){
                set(lightSource, x, -y, -z);
            }
        }
    }

    //Negative X
    for(int z = 0;z<lightSource.lightLevel;z++){
        for(int x = 0;x<max(lightSource.lightLevel-z, 0);x++){
            for(int y = 0;y<max(lightSource.lightLevel-x-z, 0);y++){
                set(lightSource, -x, -y, z);
            }
        }
    }
    for(int z = 0;z<lightSource.lightLevel;z++){
        for(int x = 0;x<max(lightSource.lightLevel-z, 0);x++){
            for(int y = 0;y<max(lightSource.lightLevel-x-z, 0);y++){
                set(lightSource, -x, -y, -z);
            }
        }
    }
}

