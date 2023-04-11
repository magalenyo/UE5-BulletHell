# UE5-BulletHell

This project is an attempt to recreate some of the bosses from Returnal, a third-person action shooter that was a PS5 launch exclusive developed by Housemarque.

Since behavior trees have become the industry standard for AI behaviors, I will be implementing the enemies' AI with BTs instead of more traditional programming.

Ixion (2nd boss)
This boss has different stages; it first starts grounded, but then immediately flies off into the sky, and the game loop starts.

Stage 1
In the first stage it will be able to perform the following actions:

Fly to: flies to a target location above the ground.
Exit attack (Basic Attack): perform a 360 degree burst of bullets that decelerate drastically using a curve.
Machine gun attack (basic attack): shoots to a predicted position of where the player will go, with a certain spread behavior.
Burst attack (Basic Attack): shoots towards the player in a cone.
Vortex attack (Heavy Attack): it shoots a cone of bullets towards the player, making a beautiful pattern.
The exit attack will always be performed before it moves to a different location.

Full video: https://www.youtube.com/watch?v=TG0Axq1YqVo


Short video
https://user-images.githubusercontent.com/27434068/231311169-f3ea8f30-a542-48d2-b306-21095bebf257.mp4